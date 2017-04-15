#pragma once
#include "karthy_GomokuAgent.h"
#include "karthy_GomokuEVE.h"

namespace karthy
{
	class GomokuSimpleAgent : public GomokuAgent
	{
	public:
		GomokuSimpleAgent(GomokuPVE* gomokuGame);
		~GomokuSimpleAgent();
		Move takeTurn(void);
		void getReady(Player myPlayer);
	};
	
}

