#include "karthy_GomokuAIAgent.h"

using namespace karthy;

karthy::GomokuAIAgent::GomokuAIAgent(GomokuPVE* pveGame, uint8_t depth)
{
	std::random_device randomDevice;
	this->generator = new std::mt19937(randomDevice());
	this->distributor = new std::bernoulli_distribution(0.5);

	this->myPhysicalGame = pveGame;
	this->depth = depth;

	this->maxActionCount = myPhysicalGame->board.colCount*myPhysicalGame->board.rowCount;
	this->myPlayer = Player::NO_PLAYER;
	this->myLogicalGame = new GomokuGame(myPhysicalGame->board.colCount, myPhysicalGame->getStoneToWin());
	this->parrentStateId = UINT64_MAX;	//null value
	this->currentStateId = UINT64_MAX;
	this->logicalVsPhysicalBoardSymmetricType = SymmetricType::ROTATE_000_FLIP_0;

	this->info.load();
}

karthy::GomokuAIAgent::~GomokuAIAgent(void)
{
	this->info.save();
	delete this->distributor;
	delete this->generator;
	delete this->myLogicalGame;
}

Move karthy::GomokuAIAgent::takeTurn(void)
{
	if (this->giveUp == true)
	{
		this->clearDecisionTree();
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

		this->currentStateId = this->locateCurrentStateId(this->myPhysicalGame->latestMove, this->myPhysicalGame->board);

		this->buildDecisionTree();

		uint16_t selectedActionOrder;
		Action* selectedAction = selectAction(this->decisionTree, selectedActionOrder);
		this->giveUp = (selectedAction->qValue == score.loseValue);
		feedbackQValue(selectedAction);
		this->previousActionOrder = selectedActionOrder;

		const Move logicalSelectedMove = Move(selectedAction->x, selectedAction->y);
		this->myLogicalGame->executeMove(logicalSelectedMove);
		this->maxActionCount--;

		this->parrentStateId = this->currentStateId;
		this->currentStateId = selectedAction->otherNode->getId();

		myPhysicalGame->eraseText(Text::THINKING);

		return convertToPhysicalMove(logicalSelectedMove);
	}
}

void karthy::GomokuAIAgent::getReady(Player myPlayer)
{
	this->myLogicalGame->newGame();
	this->currentStateId = 0;
	this->myPlayer = myPlayer;
	this->info.save();
	this->giveUp = false;
	this->maxActionCount = myPhysicalGame->board.colCount*myPhysicalGame->board.rowCount;
}

void karthy::GomokuAIAgent::buildDecisionTree(void)
{
	this->clearDecisionTree();
	this->decisionTree.root = new State(this->currentStateId, (uint8_t)this->myLogicalGame->activePlayer);
	this->addAvailableAction(this->decisionTree.root, 1);
}

void karthy::GomokuAIAgent::clearDecisionTree(void)
{
	this->decisionTree.clear();
}

void karthy::GomokuAIAgent::addAvailableAction(State* toState, uint8_t depth)
{
	if (depth == 0) { return; }
	if (toState->data->type == 3) { return; }
	if (toState->data->type == 4) { return; }

	std::ifstream loadedStateFile;
	loadedStateFile.open(KARTHY_MEMORY_PATH + to_string(toState->getId()));

	std::ofstream newStateFile;
	if (!loadedStateFile.is_open() && toState->data->type == this->decisionTree.root->data->type)
	{
		newStateFile = std::ofstream(KARTHY_MEMORY_PATH + to_string(toState->getId()));
	}
	string line;
	uint16_t maxAvailableSymmetricAction = this->maxActionCount * 2 / 3;
	if (toState->edgeList == NULL)
	{
		toState->edgeList = new forward_list<Action*>;
		for (uint8_t childX = 0; childX < this->myLogicalGame->board.colCount; childX++)
		{
			for (uint8_t childY = 0; childY < this->myLogicalGame->board.rowCount; childY++)
			{
				Index boxIndex = Index(childX, childY);
				if (this->myLogicalGame->board.getBoxStatus(boxIndex) != BoxStatus::HAVE_NO_STONE) { continue; }

				Move& nextMove = boxIndex;

				if (this->decisionTree.actionCount < maxAvailableSymmetricAction && getSymmetricType(toState->edgeList, nextMove, (BoxStatus)(toState->data->type)) != SymmetricType::NO_SYMETRIC) 
				{ 
					continue; 
				}

				Move previousLatestMove = this->myLogicalGame->latestMove;
				this->myLogicalGame->latestMove = nextMove;
				this->myLogicalGame->board.setBoxStatus(boxIndex, (BoxStatus)(toState->data->type));

				Player theWinner = this->myLogicalGame->getWinner();

				GameStatus gameStatus = GameStatus::ENDED;
				if (theWinner == Player::NO_PLAYER && !this->myLogicalGame->board.isFullBox())
				{
					gameStatus = GameStatus::PLAYING;
				}

				uint64_t stateAfterNewAvailableActionId;
				bool qValueAssigned = true;
				double qValue = score.defaultValue;
				if (loadedStateFile.is_open())
				{
					loadedStateFile >> qValue;
					loadedStateFile >> stateAfterNewAvailableActionId;
				}
				else
				{
					if (theWinner == (Player)toState->data->type)
					{
						qValue = score.winValue;
					}
					else if (this->myLogicalGame->board.isFullBox())
					{
						qValue = score.drawValue;
					}			
					else if (this->getTerminatedMove(Player(!(toState->data->type))) != NULL_MOVE)
					{
						qValue = score.loseValue;
					}

					stateAfterNewAvailableActionId = ++this->info.stateCount;

					newStateFile << qValue << '\n';
					newStateFile << stateAfterNewAvailableActionId << '\n';		
				}

				this->decisionTree.actionCount++;
				State* stateAfterNewAvailableAction = new State(stateAfterNewAvailableActionId, !toState->data->type);
				Action* newAvailableAction = new Action(nextMove.x, nextMove.y, qValue, stateAfterNewAvailableAction);
				toState->addEdge(newAvailableAction);

				if (gameStatus == GameStatus::ENDED)
				{
					newAvailableAction->otherNode->data->type = 2;
				}
				else if (depth - 1 == 0)
				{
					newAvailableAction->otherNode->data->type = 3;
				}
				else
				{
					addAvailableAction(newAvailableAction->otherNode, depth - 1);
				}

				this->myLogicalGame->latestMove = previousLatestMove;
				this->myLogicalGame->board.setBoxStatus(boxIndex, BoxStatus::HAVE_NO_STONE);
			}
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

Action* karthy::GomokuAIAgent::selectAction(DecisionTree& decisionTree, uint16_t& selectedActionOrder)
{
	Action* selectedAction = NULL;
	selectedActionOrder = 0;
	uint16_t actionCount = 0;

	bool exploit = (*this->distributor)(*this->generator);
	if (exploit)
	{
		for (forward_list<Action*>::iterator it = decisionTree.root->edgeList->begin();
			it != decisionTree.root->edgeList->end();
			++it)
		{
			if (selectedAction == NULL || selectedAction->qValue < (*it)->qValue)
			{
				selectedAction = (*it);
				selectedActionOrder = actionCount;
			}
			actionCount++;
		}
	}
	else //explore
	{
		bool flagHaveCleanPath = false;
		selectedAction = *(decisionTree.root->edgeList->begin());

		for (forward_list<Action*>::iterator it = decisionTree.root->edgeList->begin();
			it != decisionTree.root->edgeList->end();
			++it)
		{
			if (selectedAction->qValue == 0)
			{
				flagHaveCleanPath = true;
				selectedAction = (*it);
				selectedActionOrder = actionCount;
				break;
			}
			actionCount++;
		}

		if (flagHaveCleanPath == false)
		{
			std::uniform_int_distribution<> distributorAction(0, actionCount - 1);
			actionCount = 0;
			int explorationChildNode = (int)distributorAction(*this->generator);
			for (forward_list<Action*>::iterator it = decisionTree.root->edgeList->begin();
				it != decisionTree.root->edgeList->end();
				++it)
			{
				if (actionCount == explorationChildNode)
				{
					selectedAction = (*it);
					selectedActionOrder = actionCount;
					break;
				}
			}
			actionCount++;
		}
	}

	selectedActionOrder = this->decisionTree.actionCount - 1 - selectedActionOrder;
	return selectedAction;
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
	//gpu::rotate
	Mat temp = getRotationMatrix2D(ptCp, angle, 1.0);
	warpAffine(board, dst, temp, board.size(), cv::INTER_CUBIC); //Nearest is too rough 
	return dst;
}

Point2i rotatePoint90Degree(Point2i point, uint8_t boardWidth);
Point2i rotatePointAngle(Point2i point, double angle, uint8_t boardWidth)
{
	Point2i pointDst = point;
	int numberRotate90;
	if (angle == 90) numberRotate90 = 1;
	if (angle == 180) numberRotate90 = 2;
	if (angle == 270) numberRotate90 = 3;
	for (int i = 0; i < numberRotate90; ++i)
	{
		pointDst = rotatePoint90Degree(pointDst, boardWidth);
	}
	return pointDst;
}
Mat rotateBoard(Mat board, double angle)
{
	Mat dst = board.clone();
	if (angle == 0) return dst;
	for (int i = 0; i < board.cols; ++i)
	{
		for (int j = 0; j < board.rows; j++)
		{
			dst.at<uchar>(rotatePointAngle(Point2i(i, j), angle, board.cols)) = board.at<uchar>(Point2i(i, j));
		}
	}
	//printBoard(rotateBoardByOpenCV(board,angle));
	return dst;
}
Point2i flipPointVertically(Point2i point, uint8_t boardWidth);
Mat flipBoard(Mat board)
{
	Mat dst = board.clone();
	for (int i = 0; i < board.cols; ++i)
	{
		for (int j = 0; j < board.rows; j++)
		{
			dst.at<uchar>(flipPointVertically(Point2i(i, j), board.cols)) = board.at<uchar>(Point2i(i, j));
		}
	}
	return dst;
}

GomokuAIAgent::SymmetricType karthy::GomokuAIAgent::getSymmetricType(forward_list<Edge*>* currentNextNodeList, Move nextMoveToCheck, BoxStatus newBoxStatus)
{
	if (currentNextNodeList->empty() == true) { return SymmetricType::NO_SYMETRIC; }
	for (forward_list<Edge*>::iterator it = currentNextNodeList->begin(); it != currentNextNodeList->end(); ++it)
	{
		Mat existMoveBoard = this->myLogicalGame->board.boxStatus.clone();

		existMoveBoard.at<uchar>(Point2i((*it)->x, (*it)->y)) = (uchar)newBoxStatus;

		Mat futureMoveBoard = this->myLogicalGame->board.boxStatus.clone();
		futureMoveBoard.at<uchar>(Point2i(nextMoveToCheck)) = (uchar)newBoxStatus;

		SymmetricType symmetricType = getSymmetricType(existMoveBoard, futureMoveBoard);
		if (symmetricType != SymmetricType::NO_SYMETRIC)
		{
			return symmetricType;
		}
	}

	return SymmetricType::NO_SYMETRIC;
}

GomokuAIAgent::SymmetricType karthy::GomokuAIAgent::getSymmetricType(GomokuBoard& board1, GomokuBoard& board2)
{
	return getSymmetricType(board1.boxStatus, board2.boxStatus);
}

GomokuAIAgent::SymmetricType karthy::GomokuAIAgent::getSymmetricType(Mat& boxStatus1, Mat& boxStatus2)
{
	if (isEqual(boxStatus2, boxStatus1))
	{
		return SymmetricType::ROTATE_000_FLIP_0;
	}

	//rotate 90
	Mat boardRotate = rotateBoard(boxStatus1, 90);
	if (isEqual(boxStatus2, boardRotate))
	{
		return SymmetricType::ROTATE_090_FLIP_0;
	}

	//rotate 180
	boardRotate = rotateBoard(boxStatus1, 180);
	if (isEqual(boxStatus2, boardRotate))
	{
		return SymmetricType::ROTATE_180_FLIP_0;
	}

	//rotate 270
	boardRotate = rotateBoard(boxStatus1, 270);
	if (isEqual(boxStatus2, boardRotate))
	{
		return SymmetricType::ROTATE_270_FLIP_0;
	}

	Mat boardFlip = boxStatus1.clone();

	//gpu::flip(boardFlip, boxStatus1, 0);
	//0 flip Ox
	//1 flip Oy
	boardFlip = flipBoard(boxStatus1);
//	cv::flip(boxStatus1, boardFlip, 1);

	if (isEqual(boxStatus2, boardFlip))
	{
		return SymmetricType::ROTATE_000_FLIP_1;
	}

	boardRotate = rotateBoard(boardFlip, 90);
	if (isEqual(boxStatus2, boardRotate))
	{
		return SymmetricType::ROTATE_090_FLIP_1;
	}

	boardRotate = rotateBoard(boardFlip, 180);
	if (isEqual(boxStatus2, boardRotate))
	{
		return SymmetricType::ROTATE_180_FLIP_1;
	}

	boardRotate = rotateBoard(boardFlip, 270);
	if (isEqual(boxStatus2, boardRotate))
	{
		return SymmetricType::ROTATE_270_FLIP_1;
	}

	boardRotate.release();
	return SymmetricType::NO_SYMETRIC;
}

void karthy::GomokuAIAgent::estimateNode(Node* currentNode, double reward)
{
	/*
	//if (currentNode == NULL) return;

	if (currentNode == decisionTree.root)
	{
		if (currentNode->edgeList != NULL)
		{
			for (forward_list<Edge*>::iterator it = currentNode->edgeList->begin(); it != currentNode->edgeList->end(); ++it)
			{
				estimateNode((*it), reward);
			}
		}
		return;
	}
	if (currentNode->edgeList != NULL)
	{
		double maxQ = 0;
		for (forward_list<Edge*>::iterator it = currentNode->edgeList->begin(); it != currentNode->edgeList->end(); ++it)
		{
			if ((*it)->data->Q > maxQ)
			{
				maxQ = (*it)->data->Q;
			}
		}
		currentNode->data->Q = currentNode->data->Q +
			Evalute.anpha *	(reward + Evalute.grammar * maxQ - currentNode->data->Q);
	}
	else
	{
		currentNode->data->Q = currentNode->data->Q +
			Evalute.anpha *	(reward - currentNode->data->Q);
	}
	if (currentNode->edgeList != NULL)
	{
		for (forward_list<Edge*>::iterator it = currentNode->edgeList->begin(); it != currentNode->edgeList->end(); ++it)
		{
			estimateNode((*it), reward*0.9);
		}
	}
	*/
}

void karthy::GomokuAIAgent::estimateDecisionTree()
{
	/*
	for (int i = 0; i < 100; i++)
	{
		estimateNode(decisionTree.root, Score.WIN / 10.0);
	}
	*/
}

Move karthy::GomokuAIAgent::getTerminatedMove(Player player)
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

uint64_t karthy::GomokuAIAgent::locateCurrentStateId(Move& adversaryMove, GomokuBoard& physicalGameBoard)
{
	uint64_t result;

	//karthy's adversary hasn't moved because karthy go first
	if (adversaryMove == NULL_MOVE)
	{ 
		result = 0; 
	}
	//karthy's adversary has moved, karthy have to track the adversary to get adversary's move
	else 
	{
		//build adversary's decistiontree
		this->buildDecisionTree();
		State* currentState = this->decisionTree.root;

		Move logicalMove;
		forward_list<Action*>::iterator it;
		for (it = currentState->edgeList->begin(); it != currentState->edgeList->end(); ++it)
		{
			logicalMove = Move((*it)->x, (*it)->y);
			if (this->decisionTree.actionCount == this->maxActionCount)
			{
				if (convertToPhysicalMove(logicalMove) == adversaryMove)
				{
					break;
				}
			}
			else
			{
				this->myLogicalGame->board.setBoxStatus(logicalMove, (BoxStatus)!this->myPlayer);
				SymmetricType newLogicalVsPhysicalBoardSymmetricType = getSymmetricType(this->myLogicalGame->board, physicalGameBoard);
				this->myLogicalGame->board.setBoxStatus(logicalMove, BoxStatus::HAVE_NO_STONE);

				//check if got adversary's move
				if (newLogicalVsPhysicalBoardSymmetricType != SymmetricType::NO_SYMETRIC)
				{
					this->logicalVsPhysicalBoardSymmetricType = newLogicalVsPhysicalBoardSymmetricType;
					break;
				}
			}		
		}
		
		result = (*it)->otherNode->getId();
		this->myLogicalGame->executeMove(logicalMove);
		this->maxActionCount--;
	}

	return result;
}

Point2i rotatePoint90Degree(Point2i point, uint8_t boardWidth)
{
	Point2i result;

	result.x = point.y;
	result.y = -point.x + ((boardWidth >> 1) << 1);

	return result;
}

Point2i flipPointVertically(Point2i point, uint8_t boardWidth)
{
	Point2i result;

	//5 / 2 = 2. 2*2 = 4
	//5 >> 1 = 2. 2 << 1 = 4
	result.x = -point.x + ((boardWidth >> 1) << 1);
	result.y = point.y;

	return result;
}

Move karthy::GomokuAIAgent::convertToPhysicalMove(Move logicalMove)
{
	Move resultMove = logicalMove;
	if (this->logicalVsPhysicalBoardSymmetricType == SymmetricType::ROTATE_000_FLIP_0)
	{
		return resultMove;
	}

	const uint8_t boardWidth = this->myPhysicalGame->board.colCount;
	resultMove = rotatePoint90Degree(resultMove, boardWidth);
	if (this->logicalVsPhysicalBoardSymmetricType == SymmetricType::ROTATE_090_FLIP_0)
	{
		return resultMove;
	}

	resultMove = rotatePoint90Degree(resultMove, boardWidth);
	if (this->logicalVsPhysicalBoardSymmetricType == SymmetricType::ROTATE_180_FLIP_0)
	{
		return resultMove;
	}	

	resultMove = rotatePoint90Degree(resultMove, boardWidth);
	if (this->logicalVsPhysicalBoardSymmetricType == SymmetricType::ROTATE_270_FLIP_0)
	{
		return resultMove;
	}

	resultMove = flipPointVertically(logicalMove, boardWidth);
	if (this->logicalVsPhysicalBoardSymmetricType == SymmetricType::ROTATE_000_FLIP_1)
	{
		return resultMove;
	}

	resultMove = rotatePoint90Degree(resultMove, boardWidth);
	if (this->logicalVsPhysicalBoardSymmetricType == SymmetricType::ROTATE_090_FLIP_1)
	{
		return resultMove;
	}

	resultMove = rotatePoint90Degree(resultMove, boardWidth);
	if (this->logicalVsPhysicalBoardSymmetricType == SymmetricType::ROTATE_180_FLIP_1)
	{
		return resultMove;
	}

	resultMove = rotatePoint90Degree(resultMove, boardWidth);
	if (this->logicalVsPhysicalBoardSymmetricType == SymmetricType::ROTATE_270_FLIP_1)
	{
		return resultMove;
	}
}

void karthy::GomokuAIAgent::feedbackQValue(Action *currentSelectedAction)
{
	//printf("%d - %d asdas\n", this->parrentStateId, this->currentStateId);
	//= 0???
	if (this->currentStateId <= this->parrentStateId) { return; }

	string oldnameTemp = KARTHY_MEMORY_PATH + (string)"temp";
	string newnameTemp = KARTHY_MEMORY_PATH + to_string(this->parrentStateId);

	char *oldname = (char*)oldnameTemp.c_str();
	char *newname = (char*)newnameTemp.c_str();

	std::ifstream loadedParent;
	loadedParent.open(KARTHY_MEMORY_PATH + to_string(this->parrentStateId));
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
		if (previousStateActionCount == this->previousActionOrder)
		{
			//cout << i <<endl;
			previousSelectedActionQValue = previousSelectedActionQValue + this->qLearningParameter.alpha * (0 + this->qLearningParameter.gamma * currentSelectedAction->qValue - previousSelectedActionQValue);
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
	/*std::ifstream loadNewId;
	loadNewId.open(KARTHY_MEMORY_PATH + to_string(this->parrentStateId));
	int k = 0;
	while (loadNewId.good())
	{
	loadNewId >> k;
	printf("%d\n", k);
	}
	printf("/////////////////////\n");
	loadNewId.close();*/
}

