#pragma once
#include "karthy_GomokuPVE.h"
#include "karthy_GomokuAgent.h"
#include "karthy_Tree.h"
#include "karthy_AiInfo.h"

namespace karthy
{
	typedef Tree DecisionTree;
	typedef Node State;
	typedef Edge Action;

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
		
		enum class ControlSymmetric : uint8_t
		{
			Rotate0Flip0 = 0,
			Rotate90Flip0 = 1,
			Rotate180Flip0 = 2,
			Rotate270Flip0 = 3,
			Rotate0Flip1 = 4,
			Rotate90Flip1 = 5,
			Rotate180Flip1 = 6,
			Rotate270Flip1 = 7
		};

		uint64_t currentStateId;
		//parent of current node
		uint64_t parrentStateId;
		uint64_t previousActionOrder;
		//karthy's View of the game
		GomokuGame* myLogicalGame;
		AiInfo info;
		DecisionTree decisionTree;
		//the way symmetric
		ControlSymmetric way;
		bool giveUp;

		//build DecisionTree from root (the current game board state)
		void buildDecisionTree(void);

		//clear the content in DecisionTree
		void clearDecisionTree(void);

		//add availble next state node (available child node)
		void addAvailableAction(State* toState, uint8_t depth);

		//given decision tree, select next action, and return that action's order
		Action* selectAction(DecisionTree& decisionTree, uint16_t& selectedActionOrder);

		//check symmetric for reduction/mapping
		bool isSymmetric(forward_list<Edge*>* currentNextNodeList, Move nextMoveToCheck, BoxStatus newBoxStatus, bool flagUpdate = false);
		bool isSymmetric(GomokuBoard& board1, GomokuBoard& board2, bool flagUpdate = false);
		bool isSymmetric(Mat& boxStatus1, Mat& boxStatus2, bool flagUpdate = false);

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
		GomokuAIAgent(GomokuPVE* gomokuPVE, uint8_t depth);
		~GomokuAIAgent(void);

		Move takeTurn(void);
		void getReady(Player myPlayer);
	};
}
