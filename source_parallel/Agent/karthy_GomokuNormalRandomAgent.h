#pragma once
#include "karthy_GomokuAgent.h"
#include "..\Game\karthy_GomokuEVE.h"
#include <random>
#define STD_DEV 0.5
namespace karthy
{
	class GomokuNormalRandomAgent : public GomokuAgent
	{
	private:
		uint16_t emptyBoxCount;
		Move previousRandomMove;
	public:
		GomokuNormalRandomAgent(GomokuPVE* gomokuGame);
		~GomokuNormalRandomAgent();
		Move takeTurn(void);
		void getReady(Player myPlayer);
	};
}

