#pragma once
#include "..\karthy_Common.h"

namespace karthy
{
	class GomokuSimpleAnalyzer
	{
	protected:
		uint8_t numWinAIAgent;
		uint8_t numWinAgent2;
		uint8_t numMatch;
		uint32_t numAnalyze;
		uint32_t maxNumAnalyze;
	public:
		
		//nameAgent = true : AIagent
		//nameAgent = false : agent2

		uint64_t getNumWin(bool isAIAgent) ;
		virtual void increaseNumWinner(bool isAIAgent) = 0;
		virtual bool isDone(void) = 0;
	};
}

