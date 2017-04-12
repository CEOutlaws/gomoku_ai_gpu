#pragma once
#include "karthy_GomokuPVE.h"
#include "karthy_Common.h"

namespace karthy
{
	class GomokuEVE : GomokuPVE
	{
	public:
		GomokuEVE();
		GomokuEVE(uint8_t initBoardCols, uint8_t initStonesToWin, uint8_t aiDepth);
		~GomokuEVE();

		void newGame(void);
		void replay(void);
		void run(void);
	};
}
