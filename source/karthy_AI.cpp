#include "karthy_AI.h"

using namespace karthy;

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

				if (isSymmetric(toState->childList, nextMove)) { continue; }

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

bool karthy::GomokuAI::isSymmetric(forward_list<Node*>* currentNextNodeList, Move nextMoveToCheck)
{
	return false;
}