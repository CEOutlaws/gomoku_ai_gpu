#include "karthy_GomokuAIAgent.h"

karthy::GomokuAIAgent::GomokuAIAgent(GomokuPVE* pveGame, std::string infoPath, uint8_t depth)
{
	std::random_device randomDevice;
	this->generator = new std::mt19937(randomDevice());
	this->distributor = new std::bernoulli_distribution(this->exploit_epsilon);

	this->info = new AiInfo(infoPath);

	this->myPhysicalGame = pveGame;
	this->depth = depth;
	this->infoPath = infoPath;

	this->maxActionCount = BOARD_SIDE_LENGTH * BOARD_SIDE_LENGTH;
	this->myPlayer = Player::NO_PLAYER;
	this->myLogicalGame = new GomokuGame(myPhysicalGame->board.colCount, myPhysicalGame->getStoneToWin());
	this->parrentStateId = UINT64_MAX;	//null value
	this->currentStateId = UINT64_MAX;
	this->physicalVsLogicalBoardSymmetricType = SymmetricType::ROTATE_000_FLIP_0;
	this->myGpu.reset();

	this->info->load();
}

karthy::GomokuAIAgent::~GomokuAIAgent(void)
{
	this->info->save();
	delete this->info;
	delete this->distributor;
	delete this->generator;
	delete this->myLogicalGame;
}

karthy::Move karthy::GomokuAIAgent::takeTurn(void)
{
	if (this->giveUp == true)
	{
		//take turn as a simple agent
		Move result = NULL_MOVE;

		GomokuBoard& board = this->myPhysicalGame->board;
		for (uint8_t boxIndexX = 0; boxIndexX < board.colCount && result == NULL_MOVE; boxIndexX++)
		{
			for (uint8_t boxIndexY = 0; boxIndexY < board.rowCount && result == NULL_MOVE; boxIndexY++)
			{
				Index boxIndex = Index(boxIndexX, boxIndexY);
				if (board.getBoxStatus(boxIndex) == BoxStatus::HAVE_NO_STONE)
				{
					result = (Move)boxIndex;
				}
			}
		}

		return result;
	}
	else
	{
		//take turn normally
		myPhysicalGame->writeText(Text::THINKING);

		this->currentStateId = this->locateCurrentStateId(this->myPhysicalGame->latestMove);

		this->buildActionArray();

		karthy::cuda::GpuAction bestAction = this->myGpu.actionArray.getBestAction();
		feedbackQValue(bestAction);

		karthy::cuda::GpuAction selectedAction = selectAction();

		this->giveUp = (selectedAction.qValue < score.loseValue + 1);
		this->previousAction = selectedAction;
		
		const Move logicalSelectedMove = Move(selectedAction.x, selectedAction.y);
#if DEBUG
		if (this->myLogicalGame->board.getBoxStatus(logicalSelectedMove) != BoxStatus::HAVE_NO_STONE)
		{
			cout << "error3" << endl;
		}
#endif
		this->myLogicalGame->executeMove(logicalSelectedMove);
		this->myGpu.logicalMatrix.setElement(selectedAction, (int)this->myPlayer);

		this->maxActionCount--;

		this->parrentStateId = this->currentStateId;
		this->currentStateId = selectedAction.nextStateId;

		const Move physicalMove = convertToPhysicalMove(logicalSelectedMove);
		this->myGpu.physicalMatrix.setElement(physicalMove.y, physicalMove.x, (int)this->myPlayer);
#if DEBUG	
		if (this->myPhysicalGame->board.getBoxStatus(physicalMove) != BoxStatus::HAVE_NO_STONE)
		{
			cout << "error" << endl;
			cout << (int)this->physicalVsLogicalBoardSymmetricType << endl;
			printf("CPUlogical: ");
			cout << logicalSelectedMove << endl;
			this->myLogicalGame->board.print();
			printf("GPUlogical:\n");
			this->myGpu.logicalMatrix.print();
			printf("CPUphysical: ");
			cout << physicalMove << endl;
			this->myPhysicalGame->board.print();
			printf("GPUphysical:\n");
			this->myGpu.physicalMatrix.print();
			printf("");
		}	
#endif
		myPhysicalGame->eraseText(Text::THINKING);	

		return physicalMove;
	}
}

void karthy::GomokuAIAgent::getReady(Player myPlayer)
{
	this->myLogicalGame->newGame();
	this->currentStateId = 0;
	this->myPlayer = myPlayer;
	this->info->save();
	this->giveUp = false;
	//this->maxActionCount = myPhysicalGame->board.colCount*myPhysicalGame->board.rowCount;
	this->maxActionCount = BOARD_SIDE_LENGTH * BOARD_SIDE_LENGTH;
	this->myGpu.reset();
}

void karthy::GomokuAIAgent::buildActionArray(void)
{
	this->myGpu.actionArray.clearAll();
	this->myGpu.actionArray.id = this->currentStateId;
	this->myGpu.actionArray.taker = (int)this->myLogicalGame->activePlayer;
	this->addAvailableAction();
}

void karthy::GomokuAIAgent::addAvailableAction()
{
	std::ifstream loadedStateFile;
	std::ofstream newStateFile;

	loadedStateFile.open(infoPath + to_string(this->myGpu.actionArray.id));


	if (!loadedStateFile.is_open())
	{
		newStateFile = std::ofstream(infoPath + to_string(this->myGpu.actionArray.id));
	}

	string line;
	uint16_t maxAvailableSymmetricAction = this->maxActionCount * 2 / 3;

	for (uint8_t childX = 0; childX < this->myLogicalGame->board.colCount; childX++)
	{
		for (uint8_t childY = 0; childY < this->myLogicalGame->board.rowCount; childY++)
		{
			Index boxIndex = Index(childX, childY);
			if (this->myLogicalGame->board.getBoxStatus(boxIndex) != BoxStatus::HAVE_NO_STONE) { continue; }

			Move& nextMove = boxIndex;

			karthy::cuda::GpuAction newAction(childX, childY);
			if ((SymmetricType)this->myGpu.getSymmetricType(newAction, this->myGpu.logicalMatrix) != SymmetricType::NO_SYMETRIC)
			{
				continue;
			}

			Move previousLatestMove = this->myLogicalGame->latestMove;
			this->myLogicalGame->latestMove = nextMove;
			this->myLogicalGame->board.setBoxStatus(boxIndex, (BoxStatus)(this->myGpu.actionArray.taker));

			Player theWinner = this->myLogicalGame->getWinner();

			newAction.qValue = score.defaultValue;
			if (loadedStateFile.is_open())
			{
				loadedStateFile >> newAction.qValue;
				loadedStateFile >> newAction.nextStateId;
			}
			else
			{
				if (theWinner == (Player)this->myGpu.actionArray.taker)
				{
					newAction.qValue = score.winValue;
				}
				else if (this->myLogicalGame->board.isFullBox())
				{
					newAction.qValue = score.drawValue;
				}
				else if (this->getTerminatedMove(Player(!(this->myGpu.actionArray.taker))) != NULL_MOVE)
				{
					newAction.qValue = score.loseValue;
				}

				newAction.nextStateId = ++this->info->stateCount;

				newStateFile << newAction.qValue << '\n';
				newStateFile << newAction.nextStateId << '\n';
			}

			this->myGpu.actionArray.addAction(newAction);

			this->myLogicalGame->latestMove = previousLatestMove;
			this->myLogicalGame->board.setBoxStatus(boxIndex, BoxStatus::HAVE_NO_STONE);
		}
	}

	if (loadedStateFile.is_open())
	{
		loadedStateFile.close();
	}
	else
	{
		newStateFile.close();
	}
}

bool isEqual(const Mat mat1, const Mat mat2) {
	Mat diff;
	compare(mat1, mat2, diff, CMP_NE);
	int nonZero = countNonZero(diff);
	return nonZero == 0;
}

Mat rotateBoardByOpenCV(Mat board, double angle) {
	Mat dst;
	Point2i ptCp(board.cols >> 1, board.rows >> 1);
	//myGpu::rotate
	Mat temp = getRotationMatrix2D(ptCp, angle, 1.0);
	warpAffine(board, dst, temp, board.size(), cv::INTER_CUBIC); //Nearest is too rough 
	return dst;
}

karthy::Move karthy::GomokuAIAgent::getTerminatedMove(Player player)
{
	Move result = NULL_MOVE;

	for (uint8_t boxIndexX = 0; boxIndexX < this->myLogicalGame->board.colCount && result == NULL_MOVE; boxIndexX++)
	{
		for (uint8_t boxIndexY = 0; boxIndexY < this->myLogicalGame->board.rowCount && result == NULL_MOVE; boxIndexY++)
		{
			Index boxIndex = Index(boxIndexX, boxIndexY);
			if (this->myLogicalGame->board.getBoxStatus(boxIndex) == BoxStatus::HAVE_NO_STONE)
			{
				Move nextMove = boxIndex;

				Move previousLatestMove = this->myLogicalGame->latestMove;
				this->myLogicalGame->latestMove = nextMove;
				this->myLogicalGame->board.setBoxStatus(boxIndex, (BoxStatus)player);

				if (this->myLogicalGame->getWinner() != Player::NO_PLAYER)
				{
					result = nextMove;
				}

				this->myLogicalGame->latestMove = previousLatestMove;
				this->myLogicalGame->board.setBoxStatus(boxIndex, BoxStatus::HAVE_NO_STONE);
			}
		}
	}

	return result;
}

karthy::cuda::GpuAction karthy::GomokuAIAgent::selectAction()
{
	karthy::cuda::GpuAction bestAction = this->myGpu.actionArray.getBestAction();
	karthy::cuda::GpuAction exploreAction = this->myGpu.actionArray.getExploreAction();

	bool exploit = (*this->distributor)(*this->generator);
	if (exploit ||
		bestAction.qValue < score.loseValue + 1 ||
		bestAction.qValue > score.winValue - 1)
	{
		return bestAction;
	}
	else if (!exploreAction.isNull())
	{
		return exploreAction;
	}
	else
	{
		std::uniform_int_distribution<> actionDistributor(0, this->myGpu.actionArray.getElementCount() - 1);
		return this->myGpu.actionArray.getActionByIndex(actionDistributor(*this->generator));
	}
}

uint64_t karthy::GomokuAIAgent::locateCurrentStateId(const Move& adversaryMove)
{
	uint64_t result;
	karthy::cuda::GpuAction adversaryAction(adversaryMove.x, adversaryMove.y);

	//karthy's adversary hasn't moved because karthy go first
	if (adversaryMove == NULL_MOVE)
	{ 
		result = 0; 
	}
	//karthy's adversary has moved, karthy have to track the adversary to get adversary's move
	else 
	{
		this->buildActionArray();
	
		if (this->myGpu.actionArray.getElementCount() != this->maxActionCount)
		{
			this->physicalVsLogicalBoardSymmetricType = (SymmetricType)myGpu.getSymmetricType(adversaryAction, myGpu.physicalMatrix);
#if DEBUG
			if (this->physicalVsLogicalBoardSymmetricType == (SymmetricType)0)
			{
				cout << this->myGpu.actionArray.getElementCount() << endl;
				this->myGpu.actionArray.print();
				printf("CPUlogical:\n");
				this->myLogicalGame->board.print();
				printf("GPUlogical:\n");
				this->myGpu.logicalMatrix.print();
				printf("CPUphysical: ");
				cout << adversaryMove << endl;
				this->myPhysicalGame->board.print();
				printf("GPUphysical:\n");
				this->myGpu.physicalMatrix.print();
				printf("");				
			}
#endif

		}
		karthy::cuda::GpuAction logicalAction = myGpu.actionArray.searchActionSymmetric(adversaryAction, (int)this->physicalVsLogicalBoardSymmetricType);

		result = logicalAction.nextStateId;
#if DEBUG
		if (this->myLogicalGame->board.getBoxStatus(Move(logicalAction.x, logicalAction.y)) != BoxStatus::HAVE_NO_STONE)
		{
			cout << "error4" << endl;
			cout << this->myGpu.actionArray.getElementCount() << endl;
			this->myGpu.actionArray.print();
			printf("CPUlogical:");
			cout << Move(logicalAction.x, logicalAction.y) << endl;
			this->myLogicalGame->board.print();
			printf("GPUlogical:\n");
			this->myGpu.logicalMatrix.print();
			printf("CPUphysical: ");
			cout << adversaryMove << endl;
			this->myPhysicalGame->board.print();
			printf("GPUphysical:\n");
			this->myGpu.physicalMatrix.print();
			printf("");
		}
#endif
		this->myLogicalGame->executeMove(Move(logicalAction.x, logicalAction.y));
		myGpu.logicalMatrix.setElement(logicalAction, !(int)this->myPlayer);
		myGpu.physicalMatrix.setElement(adversaryAction, !(int)this->myPlayer);

		this->maxActionCount--;
	}

	return result;
}

Point2i rotatePoint90Degree(Point2i point)
{
	Point2i result;

	result.x = BOARD_SIDE_LENGTH - 1 - point.y;
	result.y = point.x;

	return result;
}

Point2i flipPointVertically(Point2i point)
{
	Point2i result;

	result.x = -point.x + BOARD_SIDE_LENGTH - 1;
	result.y = point.y;

	return result;
}

karthy::Move karthy::GomokuAIAgent::convertToPhysicalMove(const Move& logicalMove)
{
	Move resultMove = logicalMove;
	if (this->physicalVsLogicalBoardSymmetricType == SymmetricType::ROTATE_000_FLIP_0)
	{
		return resultMove;
	}

	resultMove = rotatePoint90Degree(resultMove);
	if (this->physicalVsLogicalBoardSymmetricType == SymmetricType::ROTATE_090_FLIP_0)
	{
		return resultMove;
	}

	resultMove = rotatePoint90Degree(resultMove);
	if (this->physicalVsLogicalBoardSymmetricType == SymmetricType::ROTATE_180_FLIP_0)
	{
		return resultMove;
	}	

	resultMove = rotatePoint90Degree(resultMove);
	if (this->physicalVsLogicalBoardSymmetricType == SymmetricType::ROTATE_270_FLIP_0)
	{
		return resultMove;
	}

	//flip 1 roate0
	resultMove = flipPointVertically(logicalMove);
	if (this->physicalVsLogicalBoardSymmetricType == SymmetricType::ROTATE_000_FLIP_1)
	{
		return resultMove;
	}

	//flip 1 rotate90
	resultMove = rotatePoint90Degree(resultMove);
	if (this->physicalVsLogicalBoardSymmetricType == SymmetricType::ROTATE_270_FLIP_1)
	{
		return resultMove;
	}

	//flip 1 rotate180
	resultMove = rotatePoint90Degree(resultMove);
	if (this->physicalVsLogicalBoardSymmetricType == SymmetricType::ROTATE_180_FLIP_1)
	{
		return resultMove;
	}
	//flip 1 rotate 270
	resultMove = rotatePoint90Degree(resultMove);
	if (this->physicalVsLogicalBoardSymmetricType == SymmetricType::ROTATE_090_FLIP_1)
	{
		return resultMove;
	}	
}

void karthy::GomokuAIAgent::feedbackQValue(const karthy::cuda::GpuAction& currentBestAction)
{
	//printf("%d - %d asdas\n", this->parrentStateId, this->currentStateId);
	//= 0???
	if (this->currentStateId <= this->parrentStateId) { return; }

	string oldnameTemp = infoPath + (string)"temp";
	string newnameTemp = infoPath + to_string(this->parrentStateId);

	char *oldname = (char*)oldnameTemp.c_str();
	char *newname = (char*)newnameTemp.c_str();

	std::ifstream loadedParent;
	loadedParent.open(infoPath + to_string(this->parrentStateId));
	if (!loadedParent.is_open())
	{
		printf("co loi ham feedbackQValue/n");
		return;
	}

	std::ifstream loadTemp;
	loadTemp.open(oldnameTemp);
	if (loadTemp.is_open())
	{
		remove(oldname);
	}
	loadTemp.close();
	ofstream fileout(oldnameTemp); //Temporary file

	//previou selected action's qValue
	double previousSelectedActionQValue;
	uint64_t childNode;

	//Count previous state's actions
	for (int previousStateActionCount = 0; loadedParent.good(); previousStateActionCount++)
	{
		loadedParent >> previousSelectedActionQValue;
		loadedParent >> childNode;
		if (!loadedParent.good()) break;

		if (childNode == 0)
		{
			break;
		}
		if (previousStateActionCount == this->previousAction.order)
		{
			//cout << i <<endl;
			previousSelectedActionQValue = previousSelectedActionQValue + this->qLearningParameter.alpha * (0 + this->qLearningParameter.gamma * currentBestAction.qValue - previousSelectedActionQValue);
		}

		fileout << previousSelectedActionQValue << '\n';
		fileout << childNode << '\n';
	}

	//currentNode->data->Q = currentNode->data->Q +
	//	Evalute.anpha *	(reward + Evalute.grammar * maxQ - currentNode->data->Q);
	//int reward = 0;

	// close file
	loadedParent.close();
	fileout.close();

	//rename
	if (remove(newname) != 0)
		printf("Error deleting file");

	if (rename(oldname, newname))
		printf("Error renaming file : %s - %s", oldname, newname);
}

