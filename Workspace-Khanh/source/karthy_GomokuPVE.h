#pragma once
#include "karthy_GomokuPVP.h"
#include "karthy_Tree.h"
#include "karthy_Node.h"
#include "karthy_Common.h"
#include "karthy_GomokuAI.h"

using namespace std;

namespace karthy
{	
	class GomokuAI;
	class GomokuPVE : public GomokuPVP
	{
	public:
		GomokuPVE(void);
		GomokuPVE(uint8_t initBoardCols, uint8_t initStonesToWin);
		GomokuPVE(uint8_t initBoardCols, uint8_t initStonesToWin, uint8_t aiDepth);
		~GomokuPVE(void);

		GomokuAI* karthyCEO;
		
		void newGame(void);
		void replay(void);
		void MouseHandler(int event, int x, int y);
	};
}