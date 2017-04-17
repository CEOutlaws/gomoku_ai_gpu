#pragma once
#include "karthy_GomokuAgent.h"
#include "karthy_GomokuEVE.h"
#include <random>
#define STD_DEV 1
namespace karthy
{
	class GomokuRandomAgent : public GomokuAgent
	{
	private:
		uint16_t emptyBoxCount;
		Move previousRandomMove;
	public:
		GomokuRandomAgent(GomokuPVE* gomokuGame);
		~GomokuRandomAgent();
		Move takeTurn(void);
		void getReady(Player myPlayer);
	};
}

