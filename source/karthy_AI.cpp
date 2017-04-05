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
	DecisionNode* selectedNextMove = this->decisionTree.root->childList->front();
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

				if (isSymmetric(toState->childList, nextMove, (BoxStatus)(toState->data->type))) { continue; }

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

		Mat boardRotate90 = rotateBoard(boardFutureMove, 90);
		if (isEqual(boardExistMove, boardRotate90))
		{
			return true;
		}
		//boardRotate90.release();
		
		Mat boardRotate180 = rotateBoard(boardFutureMove, 180);
		if (isEqual(boardExistMove, boardRotate180))
		{
			return true;
		}
		//boardRotate180.release();

		Mat boardRotate270 = rotateBoard(boardFutureMove, 270);
		if (isEqual(boardExistMove, boardRotate270))
		{
			return true;
		}
		//boardRotate270.release();

		Mat boardFlip  = boardFutureMove.clone(); 
		//flip(board, board, 2);
		// 0: lap theo truc dung
		// 1: theo truc ngang
		//gpu::flip(boardFlip, boardFutureMove, 0);
		flip(boardFlip, boardFutureMove, 0);
		
		if (isEqual(boardExistMove, boardFlip)) 
		{
			printBoard(boardExistMove);
			printBoard(boardFlip);
			return true;
		}

		boardRotate90 = rotateBoard(boardFlip, 90);
		if (isEqual(boardExistMove, boardRotate90))
		{
			return true;
		}
		boardRotate90.release();

		flip(boardFlip, boardFutureMove, 1);

		if (isEqual(boardExistMove, boardFlip))
		{
			return true;
		}

		boardRotate90 = rotateBoard(boardFlip, 90);
		if (isEqual(boardExistMove, boardRotate90))
		{
			return true;
		}
		boardRotate90.release();
		
	}
		
	
	return false;
}