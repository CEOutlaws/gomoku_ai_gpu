#include "karthy_gomokuPVE.h"

karthy::GomokuPVE::GomokuPVE(uint8_t initBoardCols, uint8_t initStonesToWin) :GomokuPVP::GomokuPVP(initBoardCols, initStonesToWin)
{
	karthyCEO = new GomokuAI(this, 10);
}

karthy::GomokuPVE::GomokuPVE(uint8_t initBoardCols, uint8_t initStonesToWin, uint8_t aiDepth) : GomokuPVP::GomokuPVP(initBoardCols, initStonesToWin)
{
	karthyCEO = new GomokuAI(this, aiDepth);
}
/*
karthy::gomokuPVE_c::~gomokuPVE_c()
{
}
*/
void karthy::GomokuPVE::newGame(void)
{
	GomokuPVP::newGame();
	karthyCEO->player = Player::BLACK_PLAYER;
	karthyCEO->takeTurn();
}

void karthy::GomokuPVE::replay(void)
{
	board.setAllBoxStatus(BoxStatus::HAVE_NO_STONE);
	loadMap();
	GomokuPVP::newGame();

	karthyCEO->player = !(karthyCEO->player);

	if (karthyCEO->player == Player::BLACK_PLAYER)
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
		else if (gameStatus == GameStatus::PLAYING && isPointToBoard(mouseCoor) && activePlayer != karthyCEO->player)
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

