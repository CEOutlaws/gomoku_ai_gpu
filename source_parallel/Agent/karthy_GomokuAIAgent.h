#pragma once
#include "..\Game\karthy_GomokuPVE.h"
#include "karthy_GomokuAgent.h"
#include "karthy_AiInfo.h"
#include "..\cuda\karthy_AiGpu.h"
#include <random>

namespace karthy
{
	class GomokuAgent;
	class GomokuPVE;
	class GomokuAIAgent : public GomokuAgent
	{
	protected:
		class Score
		{
		public:
			const double winValue = 1000.0;
			const double loseValue = -1000.0;
			const double drawValue = -20.0;
			const double defaultValue = 0.0;
		} score;

		class QLearningParameter
		{
		public:
			double gamma = 0.5;
			double alpha = 0.5;
		} qLearningParameter;

		enum class SymmetricType : uint8_t
		{
			NO_SYMETRIC = 0,
			ROTATE_000_FLIP_0 = 1,
			ROTATE_090_FLIP_0 = 2,
			ROTATE_180_FLIP_0 = 3,
			ROTATE_270_FLIP_0 = 4,
			ROTATE_000_FLIP_1 = 5,
			ROTATE_090_FLIP_1 = 6,
			ROTATE_180_FLIP_1 = 7,
			ROTATE_270_FLIP_1 = 8
		};
		
		double exploit_epsilon = 0.5;
		uint16_t maxActionCount;
		uint64_t currentStateId;
		//parent of current node
		uint64_t parrentStateId;
		karthy::cuda::GpuAction previousAction;
		//karthy's View of the game
		GomokuGame* myLogicalGame;
		AiInfo* info;
		//the way symmetric
		SymmetricType physicalVsLogicalBoardSymmetricType;
		bool giveUp;
		std::string infoPath;
		karthy::cuda::AiGpu myGpu;

		std::mt19937* generator;
		std::bernoulli_distribution* distributor;

		//build DecisionTree from root (the current game board state)
		void buildActionArray(void);

		void addAvailableAction();

		//if exist winning move return true
		Move getTerminatedMove(Player player);

		karthy::cuda::GpuAction selectAction();

		/*
		*precondition: before karthy make a move
		*given latest adversary's move
		*locate current state's ID
		*/
		uint64_t locateCurrentStateId(const Move& adversaryMove);
		Move convertToPhysicalMove(const Move& logicalMove);

		//update qValue
		void feedbackQValue(const karthy::cuda::GpuAction& currentSelectedAction);
	public:
		//number of move to look forward
		uint8_t depth;

		//you have to define this->player yourself
		GomokuAIAgent(GomokuPVE* gomokuPVE, std::string infoPath, uint8_t depth = 1);
		~GomokuAIAgent(void);

		Move takeTurn(void);
		void getReady(Player myPlayer);
	};
}
