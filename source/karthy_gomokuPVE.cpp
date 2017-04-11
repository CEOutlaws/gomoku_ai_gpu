#include "karthy_gomokuPVE.h"

karthy::GomokuPVE::GomokuPVE(uint8_t initBoardCols, uint8_t initStonesToWin) :GomokuPVP::GomokuPVP(initBoardCols, initStonesToWin)
{
	this->karthyCEO = new GomokuAI(this, 10);
}

karthy::GomokuPVE::GomokuPVE(uint8_t initBoardCols, uint8_t initStonesToWin, uint8_t aiDepth) : GomokuPVP::GomokuPVP(initBoardCols, initStonesToWin)
{
	this->karthyCEO = new GomokuAI(this, aiDepth);
}

karthy::GomokuPVE::~GomokuPVE(void)
{
	delete this->karthyCEO;
}

void karthy::GomokuPVE::newGame(void)
{
	GomokuPVP::newGame();
	karthyCEO->getReady(Player::BLACK_PLAYER);
	this->karthyCEO->takeTurn();
}

void karthy::GomokuPVE::replay(void)
{
	loadMap();
	GomokuPVP::newGame();
	Player karthyPreviousPlayer = karthyCEO->getPlayer();
	karthyCEO->getReady(!karthyPreviousPlayer);

	if (karthyCEO->getPlayer() == Player::BLACK_PLAYER)
	{
		karthyCEO->takeTurn();
	}
}

void karthy::GomokuPVE::MouseHandler(int event, int x, int y)
{
	if (event == EVENT_LBUTTONDOWN ||
		event == EVENT_RBUTTONDOWN ||
		event == EVENT_MBUTTONDOWN)
	{
		//if in Replay Icon
		const Point2i mouseCoor(x, y);
		if (isPointToReplayIcon(mouseCoor))
		{
			replay();
		}
		else if (gameStatus == GameStatus::PLAYING && isPointToBoard(mouseCoor) && activePlayer != karthyCEO->getPlayer())
		{
			Index pointedBoxIndex = GetBoxIndex(mouseCoor);

			if (board.getBoxStatus(pointedBoxIndex) == BoxStatus::HAVE_NO_STONE)
			{	
				//Human move
				executeMove(pointedBoxIndex);
				if (gameStatus == GameStatus::ENDED) return;

				//Karthy take her turn
				karthyCEO->takeTurn();
				//Human take their turn
			}
		}
	}
}

