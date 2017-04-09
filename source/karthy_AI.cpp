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
	player = Player::NO_PLAYER;
	_game = pveGame;
	this->depth = depth;
}

karthy::GomokuAI::~GomokuAI()
{
}

void karthy::GomokuAI::takeTurn(void)
{
	String text = "Thinking";
	Point textOrg = Point2i(790, 130);
	int fontFace = FONT_HERSHEY_PLAIN;
	double fontScale = 6;
	int thickness = 5;
	cv::putText(_game->gameFrame, text, textOrg, fontFace, fontScale, { 255, 0, 0 }, thickness, 8);
	_game->showMap();

	this->buildDecisionTree();

	/////this policy is going to be replaced with Qlearning

	estimateDecisionTree();
	DecisionNode* selectedNextMove = selectBestQ();
	//DecisionNode* selectedNextMove = this->decisionTree.root->childList->front();

	//////////////////////////////

	cv::putText(_game->gameFrame, text, textOrg, fontFace, fontScale, BACKGROUND_COLOR, thickness, 8);
	_game->executeMove(Index(selectedNextMove->data->x, selectedNextMove->data->y));

	this->clearDecisionTree();
}

void karthy::GomokuAI::buildDecisionTree(void)
{
	this->decisionTree.root = new DecisionNode(_game->latestMove.x, _game->latestMove.y, (uint8_t)this->player, 0);
	this->addAvailableNextNode(this->decisionTree.root, this->depth);
}

void karthy::GomokuAI::clearDecisionTree(void)
{
	this->decisionTree.clear();
}

void karthy::GomokuAI::addAvailableNextNode(DecisionNode* toState, uint8_t depth)
{
	if (depth == 0) { return; }
	if (toState->childList == NULL)
	{
		toState->childList = new forward_list<DecisionNode*>;
		for (uint8_t childX = 0; childX < _game->board.colCount; childX++)
		{
			for (uint8_t childY = 0; childY < _game->board.rowCount; childY++)
			{
				Index boxIndex = Index(childX, childY);
				if (_game->board.getBoxStatus(boxIndex) != BoxStatus::HAVE_NO_STONE) { continue; }

				Move& nextMove = boxIndex;

				if (isSymmetric(toState->childList, nextMove, (BoxStatus)(toState->data->type))) { printf("OK\n"); continue; }

				_game->latestMove = nextMove;
				_game->board.setBoxStatus(boxIndex, (BoxStatus)(toState->data->type));

				double qValue = Score.DEFAULT;
				GameStatus gameStatus = GameStatus::ENDED;
				Player theWinner = _game->getWinner();
				if (theWinner != Player::NO_PLAYER)
				{
					if (theWinner == this->player)
					{
						qValue = Score.WIN;
					}
					else
					{
						qValue = Score.LOSE;
					}
				}
				else if (_game->board.isFullBox())
				{
					qValue = Score.DRAW;
				}
				else
				{
					gameStatus = GameStatus::PLAYING;
				}

				DecisionNode* nextDecisionNode = new DecisionNode(nextMove.x, nextMove.y, !toState->data->type, qValue);
				toState->addChild(nextDecisionNode);

				if (gameStatus == GameStatus::ENDED)
				{
					nextDecisionNode->data->type = 2;
				}
				else if (depth - 1 == 0)
				{
					nextDecisionNode->data->type = 3;
				}
				else
				{
					addAvailableNextNode(nextDecisionNode, depth - 1);
				}

				_game->latestMove = Move(toState->data->x, toState->data->y);
				_game->board.setBoxStatus(boxIndex, BoxStatus::HAVE_NO_STONE);
			}
		}
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
bool karthy::GomokuAI::isSymmetric(forward_list<Node*>* currentNextNodeList, Move nextMoveToCheck, BoxStatus newBoxStatus)
{
	if (currentNextNodeList->empty() == true) return false;
	for (forward_list<Node*>::iterator it = currentNextNodeList->begin(); it != currentNextNodeList->end(); ++it)
	{
		// board cho buoc di da duoc add
		Mat boardExistMove = _game->board.boxStatus.clone();

		boardExistMove.at<uchar>(Point2i((*it)->data->x,(*it)->data->y)) = (uchar)newBoxStatus;
		
		Mat boardFutureMove = _game->board.boxStatus.clone();
		boardFutureMove.at<uchar>(Point2i(nextMoveToCheck)) = (uchar)newBoxStatus;

		/*
		debug
		printBoard(boardExistMove);
		printBoard(boardFutureMove);
		printf("/////////\n");*/
		//rotate 90
		Mat boardRotate = rotateBoard(boardFutureMove, 90);
		if (isEqual(boardExistMove, boardRotate))
		{
			return true;
		}
		//boardRotate90.release();
		
		//rotate 180
		boardRotate = rotateBoard(boardFutureMove, 90);
		if (isEqual(boardExistMove, boardRotate))
		{
			return true;
		}

		//rotate 270
		boardRotate = rotateBoard(boardFutureMove, 90);
		if (isEqual(boardExistMove, boardRotate))
		{
			return true;
		}

		Mat boardFlip  = boardFutureMove.clone(); 
		//flip(board, board, 2);
		// 0: lap theo truc dung
		// 1: theo truc ngang
		//gpu::flip(boardFlip, boardFutureMove, 0);
		flip(boardFutureMove, boardFlip, 0);
		/*printf("*******\n");
		printBoard(boardFlip);
		printf("*******\n");*/
		if (isEqual(boardExistMove, boardFlip)) 
		{
			return true;
		}

		boardRotate = rotateBoard(boardFlip, 90);
		if (isEqual(boardExistMove, boardRotate))
		{
			return true;
		}

		boardRotate = rotateBoard(boardFlip, 180);

		if (isEqual(boardExistMove, boardRotate))
		{
			return true;
		}

		boardRotate = rotateBoard(boardFlip, 270);
		if (isEqual(boardExistMove, boardRotate))
		{
			return true;
		}
		boardRotate.release();
		
	}
		
	
	return false;
}
void karthy::GomokuAI::estimateNode(Node* currentNode, double reward)
{
	//if (currentNode == NULL) return;

	if (currentNode == decisionTree.root)
	{
		if (currentNode->childList != NULL)
		{
			for (forward_list<Node*>::iterator it = currentNode->childList->begin(); it != currentNode->childList->end(); ++it)
			{
				estimateNode((*it), reward);
			}
		}
		return;
	}
	if (currentNode->childList!=NULL) 
	{
		double maxQ = 0;
		for (forward_list<Node*>::iterator it = currentNode->childList->begin(); it != currentNode->childList->end(); ++it)
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
	if (currentNode->childList!=NULL)
	{
		for (forward_list<Node*>::iterator it = currentNode->childList->begin(); it != currentNode->childList->end(); ++it)
		{
			estimateNode((*it), reward*0.9);
		}
	}
}
void karthy::GomokuAI::estimateDecisionTree()
{
	for (int i = 0; i < 100; i++)
	{
		estimateNode(decisionTree.root, Score.WIN / 10.0);
	}
}
karthy::GomokuAI::DecisionNode* karthy::GomokuAI::selectBestQ()
{
	DecisionNode *bestNode = NULL;
	
	if (!decisionTree.root->childList->empty())
	{
		for (forward_list<Node*>::iterator it = decisionTree.root->childList->begin(); it != decisionTree.root->childList->end(); ++it)
		{
			if (bestNode==NULL)
			{
				bestNode = (*it);
			}
			else if ((*it)->data->Q > bestNode->data->Q)
			{
				bestNode = (*it);
			}
		}
	}
	else
	{
		printf("Error on selectBestQ");
	}
	return bestNode;
}