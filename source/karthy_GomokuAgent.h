#pragma once
#include "karthy_Common.h"

namespace karthy
{
	class GomokuPVE;
	class GomokuAIAgent;

	class GomokuAgent
	{	
	protected: 
		GomokuPVE* myPhysicalGame;
		//agent's playing role
		Player myPlayer;
	public:
		virtual Move takeTurn(void) = 0;
		virtual void getReady(Player myPlayer) = 0;
		virtual Player getPlayer(void);
	};
}

