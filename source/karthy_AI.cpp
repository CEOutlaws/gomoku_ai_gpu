#include "karthy_AI.h"

using namespace karthy;

void printBoard(Mat board)
{
	for (int i = 0; i < board.cols; i++)
	{
		for (int j = 0; j < board.rows; j++)
		{
			printf("%d", board.at<uchar>(Point2i(i, j)));
		}
		printf("\n");
	}
}

karthy::GomokuAI::GomokuAI(GomokuPVE* pveGame, uint8_t depth)
{
	this->myPlayer = Player::NO_PLAYER;
	this->_game = pveGame;
	this->depth = depth;
	this->myGame = new GomokuGame(_game->board.colCount, _game->getStoneToWin());

	ifstream infoFile;
	infoFile.open((string)KARTHY_MEMORY_PATH + "info");
	if (!infoFile.is_open())
	{
		this->stateCount = 0;
	}
	else
	{
		infoFile >> this->stateCount;
		infoFile.close();
	}
}

karthy::GomokuAI::~GomokuAI(void)
{
	ofstream infoFile((string)KARTHY_MEMORY_PATH + "info");
	infoFile << this->stateCount;
	infoFile.close();

	delete this->myGame;
}

void karthy::GomokuAI::takeTurn(void)
{
	const String text = "Thinking";
	const Point textOrg = Point2i(790, 130);
	const int fontFace = FONT_HERSHEY_PLAIN;
	const double fontScale = 6;
	const int thickness = 5;
	cv::putText(_game->gameFrame, text, textOrg, fontFace, fontScale, { 255, 0, 0 }, thickness, 8);
	_game->showMap();

	this->currentStateId = this->locateCurrentStateId(this->_game->latestMove, this->_game->board);

	this->buildDecisionTree();

	//estimateDecisionTree();
 	Action* selectedAction = selectAction(this->decisionTree);
	//DecisionNode* selectedNextMove = this->decisionTree.root->childList->front();
	
	const Move logicalSelectedMove = Move(selectedAction->x, selectedAction->y);
	this->currentStateId = selectedAction->otherNode->getId();
	this->myGame->executeMove(logicalSelectedMove);

	const Move physicalSelectedMove = convertToPhysicalMove(logicalSelectedMove);
	cv::putText(_game->gameFrame, text, textOrg, fontFace, fontScale, BACKGROUND_COLOR, thickness, 8);
	this->_game->executeMove(physicalSelectedMove);
}

void karthy::GomokuAI::getReady(Player myPlayer)
{
	this->myGame->newGame();
	this->currentStateId = 0;
	this->myPlayer = myPlayer;
}

Player karthy::GomokuAI::getPlayer(void)
{
	return this->myPlayer;
}

void karthy::GomokuAI::buildDecisionTree(void)
{
	this->clearDecisionTree();
	this->decisionTree.root = new State(this->currentStateId, (uint8_t)this->myGame->activePlayer);
	this->addAvailableAction(this->decisionTree.root, 1);
}

void karthy::GomokuAI::clearDecisionTree(void)
{
	this->decisionTree.clear();
}

void karthy::GomokuAI::addAvailableAction(State* toState, uint8_t depth)
{
	if (depth == 0) { return; }

	std::ifstream loadedStateFile;
	loadedStateFile.open(KARTHY_MEMORY_PATH + to_string(toState->getId()));

	std::ofstream newStateFile;
	if (!loadedStateFile.is_open())
	{
		newStateFile = std::ofstream(KARTHY_MEMORY_PATH + to_string(toState->getId()));
	}
	string line;

	if (toState->edgeList == NULL)
	{
		toState->edgeList = new forward_list<Action*>;
		for (uint8_t childX = 0; childX < this->myGame->board.colCount; childX++)
		{
			for (uint8_t childY = 0; childY < this->myGame->board.rowCount; childY++)
			{
				Index boxIndex = Index(childX, childY);
				if (this->myGame->board.getBoxStatus(boxIndex) != BoxStatus::HAVE_NO_STONE) { continue; }

				Move& nextMove = boxIndex;

				if (isSymmetric(toState->edgeList, nextMove, (BoxStatus)(toState->data->type))) { continue; }

				Move previousLatestMove = this->myGame->latestMove;
				this->myGame->latestMove = nextMove;
				this->myGame->board.setBoxStatus(boxIndex, (BoxStatus)(toState->data->type));

				Player theWinner = this->myGame->getWinner();

				GameStatus gameStatus = GameStatus::ENDED;
				if (theWinner == Player::NO_PLAYER && !this->myGame->board.isFullBox())
				{
					gameStatus = GameStatus::PLAYING;
				}

				uint64_t stateAfterNewAvailableActionId;
				double qValue = Score.DEFAULT;
				if (loadedStateFile.is_open())
				{
					loadedStateFile >> qValue;
					loadedStateFile >> stateAfterNewAvailableActionId;
				}
				else
				{
					if (theWinner != Player::NO_PLAYER)
					{
						if (theWinner == this->myPlayer)
						{
							qValue = Score.WIN;
						}
						else
						{
							qValue = Score.LOSE;
						}
					}
					else if (this->myGame->board.isFullBox())
					{
						qValue = Score.DRAW;
					}

					stateAfterNewAvailableActionId = ++stateCount;

					newStateFile << qValue << '\n';
					newStateFile << stateAfterNewAvailableActionId << '\n';
				}
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

				this->myGame->latestMove = previousLatestMove;
				this->myGame->board.setBoxStatus(boxIndex, BoxStatus::HAVE_NO_STONE);
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

bool isEqual(const Mat mat1, const Mat mat2) {
	Mat diff;
	compare(mat1, mat2, diff, CMP_NE);
	int nonZero = countNonZero(diff);
	return nonZero == 0;
}

Mat rotateBoard(Mat board, double angle) {
	Mat dst;
	Point2i ptCp(board.cols*0.5, board.rows*0.5);
	//gpu::rotate
	Mat temp = getRotationMatrix2D(ptCp, angle, 1.0);
	warpAffine(board, dst, temp, board.size(), cv::INTER_CUBIC); //Nearest is too rough 
	return dst;
}

bool karthy::GomokuAI::isSymmetric(forward_list<Edge*>* currentNextNodeList, Move nextMoveToCheck, BoxStatus newBoxStatus)
{
	if (currentNextNodeList->empty() == true) return false;
	for (forward_list<Edge*>::iterator it = currentNextNodeList->begin(); it != currentNextNodeList->end(); ++it)
	{
		Mat boardExistMove = this->myGame->board.boxStatus.clone();

		boardExistMove.at<uchar>(Point2i((*it)->x, (*it)->y)) = (uchar)newBoxStatus;

		Mat boardFutureMove = this->myGame->board.boxStatus.clone();
		boardFutureMove.at<uchar>(Point2i(nextMoveToCheck)) = (uchar)newBoxStatus;

		if (isSymmetric(boardExistMove, boardFutureMove))
		{
			return true;
		}
	}

	return false;
}

bool karthy::GomokuAI::isSymmetric(GomokuBoard& board1, GomokuBoard& board2)
{
	return isSymmetric(board1.boxStatus, board2.boxStatus);
}


bool karthy::GomokuAI::isSymmetric(Mat& boxStatus1, Mat& boxStatus2)
{
	/*
	debug
	printBoard(boxStatus2);
	printBoard(boxStatus1);
	printf("/////////\n");
	*/

	if (isEqual(boxStatus2, boxStatus1))
	{
		return true;
	}

	//rotate 90
	Mat boardRotate = rotateBoard(boxStatus1, 90);
	if (isEqual(boxStatus2, boardRotate))
	{
		return true;
	}
	//boardRotate90.release();

	//rotate 180
	boardRotate = rotateBoard(boxStatus1, 180);
	if (isEqual(boxStatus2, boardRotate))
	{
		return true;
	}

	//rotate 270
	boardRotate = rotateBoard(boxStatus1, 270);
	if (isEqual(boxStatus2, boardRotate))
	{
		return true;
	}

	Mat boardFlip = boxStatus1.clone();
	//flip(board, board, 2);
	// 0: lap theo truc dung
	// 1: theo truc ngang
	//gpu::flip(boardFlip, boxStatus1, 0);
	flip(boxStatus1, boardFlip, 0);
	/*printf("*******\n");
	printBoard(boardFlip);
	printf("*******\n");*/
	if (isEqual(boxStatus2, boardFlip))
	{
		return true;
	}

	boardRotate = rotateBoard(boardFlip, 90);
	if (isEqual(boxStatus2, boardRotate))
	{
		return true;
	}

	boardRotate = rotateBoard(boardFlip, 180);

	if (isEqual(boxStatus2, boardRotate))
	{
		return true;
	}

	boardRotate = rotateBoard(boardFlip, 270);
	if (isEqual(boxStatus2, boardRotate))
	{
		return true;
	}

	boardRotate.release();
	return false;
}

void karthy::GomokuAI::estimateNode(Node* currentNode, double reward)
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

void karthy::GomokuAI::estimateDecisionTree()
{
	/*
	for (int i = 0; i < 100; i++)
	{
		estimateNode(decisionTree.root, Score.WIN / 10.0);
	}
	*/
}

uint64_t karthy::GomokuAI::locateCurrentStateId(Move& adversaryMove, GomokuBoard& gameBoard)
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

		for (forward_list<Action*>::iterator it = currentState->edgeList->begin(); it != currentState->edgeList->end(); ++it)
		{
			const Move move = Move((*it)->x, (*it)->y);
			this->myGame->board.setBoxStatus(move, (BoxStatus)!this->myPlayer);

			bool symetric = isSymmetric(this->myGame->board, gameBoard);

			this->myGame->board.setBoxStatus(move, BoxStatus::HAVE_NO_STONE);

			//check if got adversary's move
			if (symetric)
			{
				result = (*it)->otherNode->getId();
				this->myGame->executeMove(move);
				break;
			}
		}
	}

	return result;
}

karthy::GomokuAI::Action* karthy::GomokuAI::selectAction(DecisionTree& decisionTree)
{
	Action* action = NULL;

	if (!decisionTree.root->edgeList->empty())
	{
		for (forward_list<Action*>::iterator it = decisionTree.root->edgeList->begin(); it != decisionTree.root->edgeList->end(); ++it)
		{
			if (action == NULL)
			{
				action = (*it);
			}
			else if ((*it)->qValue > action->qValue)
			{
				action = (*it);
			}
		}
	}
	else
	{
		cout << "No action available" << endl;
	}

	return action;
}

Move karthy::GomokuAI::convertToPhysicalMove(Move logicalMove)
{
	//just for fun :3
	Move physicalMove = logicalMove;
	return physicalMove;
}
