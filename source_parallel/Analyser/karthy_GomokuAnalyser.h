#pragma once
#include "karthy_Analyser.h"
#include "..\karthy_Common.h"
#include "time.h"

namespace karthy
{
	class GomokuSimpleAnalyzer;
	class GomokuAnalyzer : public GomokuSimpleAnalyzer
	{
	protected:
		bool flagAnalazing;
		uint8_t numDrawMatch;
		std::ofstream database;
		clock_t timeStart;
	public:
		GomokuAnalyzer(bool flag, uint16_t maxNumAnalyze = 1000);
		~GomokuAnalyzer();
		bool getFlagAnalazing();
		void increaseNumWinner(bool isAIAgent);
		void increaseNumDrawMatch();
		void statisticWin();
		bool isDone(void);
	};
}
