#pragma once
#include "karthy_Common.h"

namespace karthy
{
	class GomokuSimpleAnalyzer
	{
	protected:
		uint64_t numWinAIAgent;
		uint64_t numWinAgent2;
		uint64_t numMatch;
	public:
		
		//nameAgent = true : AIagent
		//nameAgent = false : agent2

		uint64_t getNumWin(bool isAIAgent) ;
		virtual void increaseNumWinner(bool isAIAgent) = 0;
	};
}

