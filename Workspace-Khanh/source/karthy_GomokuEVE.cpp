#include "karthy_GomokuEVE.h"

using namespace karthy;

karthy::GomokuEVE::GomokuEVE()
{
}

karthy::GomokuEVE::GomokuEVE(uint8_t initBoardCols, uint8_t initStonesToWin, uint8_t aiDepth) : GomokuPVE(initBoardCols, initStonesToWin, aiDepth)
{
}


karthy::GomokuEVE::~GomokuEVE()
{
}

void karthy::GomokuEVE::newGame(void)
{
	GomokuPVE::newGame();
}

void karthy::GomokuEVE::replay(void)
{
	GomokuPVE::replay();
}

void karthy::GomokuEVE::run(void)
{
	this->initGUI();
	this->newGame();
	return;
	while (1)
	{
		this->executeMove(karthyCEO->takeTurn());
		this->executeMove(karthyCEO->takeTurn());
	}

}
