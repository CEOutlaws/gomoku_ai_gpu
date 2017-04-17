#pragma once
#include "karthy_GomokuAgent.h"
#include "karthy_GomokuEVE.h"
#include <random>

namespace karthy
{
	class GomokuUniformRandomAgent : public GomokuAgent
	{
	private:
		uint16_t emptyBoxCount;
		
	public:
		GomokuUniformRandomAgent(GomokuPVE* gomokuGame);
		~GomokuUniformRandomAgent();
		Move takeTurn(void);
		void getReady(Player myPlayer);
	};
}

