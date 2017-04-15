#pragma once
#include "karthy_GomokuPVE.h"
#include "karthy_GomokuRandomAgent.h"
#include "karthy_GomokuSimpleAgent.h"
#include "karthy_Common.h"

namespace karthy
{
	class GomokuEVE : GomokuPVE
	{
	private:
		GomokuAgent* adversaryAgent;
	public:
		GomokuEVE();
		GomokuEVE(uint8_t initBoardCols, uint8_t initStonesToWin, uint8_t aiDepth);
		~GomokuEVE();
		void newGame(void);
		void replay(void);
		void run(void);
		void executeMove(Move move);
		void MouseHandler(int event, int x, int y);
	};
}
