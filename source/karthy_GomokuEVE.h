#pragma once
#include "karthy_GomokuPVE.h"
#include "karthy_GomokuUniformRandomAgent.h"
#include "karthy_GomokuNormalRandomAgent.h"
#include "karthy_GomokuSimpleAgent.h"
#include "karthy_GomokuAiAgent.h"
#include "karthy_Common.h"
#include "karthy_GomokuAnalyser.h"

#define WAIT_MOVE_TIME (10)

namespace karthy
{
	class GomokuAnalyzer;
	enum class AgentType : uint8_t
	{
		SIMPLE = 0,
		UNIFORM_RANDOM = 1,
		AI = 2,
		NORMAL_RANDOM = 3
	};

	class GomokuEVE : GomokuPVE
	{
	private:
		bool analyzing;
		GomokuAnalyzer* analyserAgent;
		GomokuAgent* adversaryAgent;
	public:
		GomokuEVE();
		//GomokuEVE(uint8_t initBoardCols, uint8_t initStonesToWin, uint8_t aiDepth =1
		//, AgentType adversaryAgentType = AgentType::SIMPLE, GomokuAnalyser* initAnalyser);
		GomokuEVE(uint8_t initBoardCols, uint8_t initStonesToWin
			, uint8_t aiDepth, AgentType adversaryAgentType, GomokuAnalyzer* initAnalyser);
		~GomokuEVE();
		void newGame(void);
		void replay(void);
		void run(void);
		void executeMove(Move move);
		void MouseHandler(int event, int x, int y);
	};
}
