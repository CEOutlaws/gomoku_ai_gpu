#pragma once
#include "karthy_GomokuPVE.h"
#include "karthy_GomokuAgent.h"
#include "karthy_DecisionTree.h"
#include "karthy_AiInfo.h"
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
			const double drawValue = 0.0;
			const double defaultValue = 0.0;
		} score;

		class QLearningParameter
		{
		public:
			double gamma = 0.9;
			double alpha = 0.9;
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
		
		uint16_t maxActionCount;
		uint64_t currentStateId;
		//parent of current node
		uint64_t parrentStateId;
		uint64_t previousActionOrder;
		//karthy's View of the game
		GomokuGame* myLogicalGame;
		AiInfo info;
		DecisionTree decisionTree;
		//the way symmetric
		SymmetricType logicalVsPhysicalBoardSymmetricType;
		bool giveUp;

		std::mt19937* generator;
		std::bernoulli_distribution* distributor;

		//build DecisionTree from root (the current game board state)
		void buildDecisionTree(void);

		//clear the content in DecisionTree
		void clearDecisionTree(void);

		//add availble next state node (available child node)
		void addAvailableAction(State* toState, uint8_t depth);

		//given decision tree, select next action, and return that action's order
		Action* selectAction(DecisionTree& decisionTree, uint16_t& selectedActionOrder);
	
		//check symmetric for reduction/mapping
		SymmetricType getSymmetricType(forward_list<Edge*>* currentNextNodeList, Move nextMoveToCheck, BoxStatus newBoxStatus);
		SymmetricType getSymmetricType(GomokuBoard& board1, GomokuBoard& board2);
		SymmetricType getSymmetricType(Mat& boxStatus1, Mat& boxStatus2);

		//calculate Q match
		void estimateNode(Node* currentNode, double reward);
		void estimateDecisionTree(void);

		//if exist winning move return true
		Move getTerminatedMove(Player player);
		/*
		*given latest adversary's move, current game's board
		*locate current state's ID when karthy take turn
		*/
		uint64_t locateCurrentStateId(Move& adversaryMove, GomokuBoard& gameBoard);
		Move convertToPhysicalMove(Move logicalMove);

		//debug function

		//update qValue
		void feedbackQValue(Action* currentSelectedAction);
	public:
		//number of move to look forward
		uint8_t depth;

		//you have to define this->player yourself
		GomokuAIAgent(GomokuPVE* gomokuPVE, uint8_t depth = 1);
		~GomokuAIAgent(void);

		Move takeTurn(void);
		void getReady(Player myPlayer);
	};
}
