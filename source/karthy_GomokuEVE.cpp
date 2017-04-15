#include "karthy_GomokuEVE.h"

using namespace karthy;

karthy::GomokuEVE::GomokuEVE()
{
}

karthy::GomokuEVE::GomokuEVE(uint8_t initBoardCols, uint8_t initStonesToWin, uint8_t aiDepth) : GomokuPVE(initBoardCols, initStonesToWin, aiDepth)
{
	//adversaryAgent = new GomokuSimpleAgent(this);
	adversaryAgent = new GomokuRandomAgent(this);
}


karthy::GomokuEVE::~GomokuEVE()
{
	delete adversaryAgent;
}

void karthy::GomokuEVE::newGame(void)
{
	GomokuPVE::newGame();
	adversaryAgent->getReady(Player::WHITE_PLAYER);

	while (1)
	{
		this->executeMove(adversaryAgent->takeTurn());
		this->executeMove(karthyCEO->takeTurn());
	}
}

void karthy::GomokuEVE::replay(void)
{
	GomokuPVE::replay();
	Player adversaryPreviousPlayer = adversaryAgent->getPlayer();
	adversaryAgent->getReady(!adversaryPreviousPlayer);

	while (1)
	{
		this->executeMove(adversaryAgent->takeTurn());
		this->executeMove(karthyCEO->takeTurn());
	}
}

void karthy::GomokuEVE::run(void)
{
	this->initGUI();
	this->newGame();	
}

void karthy::GomokuEVE::executeMove(Move move)
{
	GomokuPVE::executeMove(move);
	//GomokuGame::executeMove(move);

	if (this->gameStatus == GameStatus::ENDED)
	{
		replay();
		waitKey(0);
	}
	waitKey(0);
}

void karthy::GomokuEVE::MouseHandler(int event, int x, int y)
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
	}
}
