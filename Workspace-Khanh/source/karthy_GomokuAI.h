#pragma once
#include "karthy_GomokuAgent.h"

namespace karthy
{
	class GomokuAI : public GomokuAgent
	{
	protected:
		class Score
		{
		public:
			const double WIN = 1000.0;
			const double LOSE = -1000.0;
			const double DRAW = 0.0;
			const double DEFAULT = 0.0;
		} Score;

		class QLearningParameter
		{
		public:
			double grammar = 0.9;
			double anpha = 0.9;
		} qLearningParameter;

		uint64_t currentStateId;
		//karthy's View of the game
		GomokuGame* myGame;
		AiInfo info;

		//build DecisionTree from root (the current game board state)
		void buildDecisionTree(void);

		//clear the content in DecisionTree
		void clearDecisionTree(void);

		//add availble next state node (available child node)
		void addAvailableAction(State* toState, uint8_t depth);

		//given decision tree, select next action
		Action* selectAction(DecisionTree& decisionTree);

		//check symmetric for reduction/mapping
		bool isSymmetric(forward_list<Edge*>* currentNextNodeList, Move nextMoveToCheck, BoxStatus newBoxStatus);
		bool isSymmetric(GomokuBoard& board1, GomokuBoard& board2);
		bool isSymmetric(Mat& boxStatus1, Mat& boxStatus2);

		//calculate Q match
		void estimateNode(Node* currentNode, double reward);
		void estimateDecisionTree(void);

		/*
		*given latest adversary's move, current game's board
		*locate current state's ID when karthy take turn
		*/
		uint64_t locateCurrentStateId(Move& adversaryMove, GomokuBoard& gameBoard);
		Move convertToPhysicalMove(Move logicalMove);
	public:
		//number of move to look forward
		uint8_t depth;

		//you have to define this->player yourself
		GomokuAI(GomokuPVE* gomokuPVE, uint8_t depth);
		~GomokuAI(void);

		Move takeTurn(void);
		void getReady(Player myPlayer);
		Player getPlayer(void);
	};
}
