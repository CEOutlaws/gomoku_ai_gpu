#pragma once
#include "karthy_gomokuPVP.h"
#include "karthy_tree.h"
#include "karthy_node.h"
#include "karthy_common.h"
#include "karthy_AI.h"

using namespace std;

namespace karthy
{	
	class GomokuAI;
	class GomokuPVE : public GomokuPVP
	{
	public:

		GomokuPVE(uint8_t initBoardCols, uint8_t initStonesToWin);
		GomokuPVE(uint8_t initBoardCols, uint8_t initStonesToWin, uint8_t aiDepth);
		~GomokuPVE(void);

		GomokuAI* karthyCEO;
		
		void newGame(void);
		void replay(void);
		void MouseHandler(int event, int x, int y);
	};
}