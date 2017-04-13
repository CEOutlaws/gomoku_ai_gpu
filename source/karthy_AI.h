#pragma once
#include "karthy_common.h"
#include "karthy_tree.h"
#include "karthy_node.h"
#include "karthy_nodedata.h"
#include "karthy_edge.h"
#include "karthy_gomokuPVE.h"

namespace karthy
{
	class GomokuPVE;
	class GomokuAI
	{
		typedef Tree DecisionTree;
		typedef Node State;
		typedef Edge Action;
	private:
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
		}qLearningParameter;

		GomokuPVE* _game;
		DecisionTree decisionTree;
		uint64_t currentStateId;
		uint64_t stateCount;
		//parent of current node
		uint64_t parrentStateId;
		uint64_t oldAction;
		//karthy's View of the game
		GomokuGame* myGame;
		//karthy's playing role
		Player myPlayer;

		//build DecisionTree from root (the current game board state)
		void buildDecisionTree(void);

		//clear the content in DecisionTree
		void clearDecisionTree(void);

		//add availble next state node (available child node)
		void addAvailableAction(State* toState, uint8_t depth);

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
	
		//given decision tree, select next action
		Action* selectAction(DecisionTree& decisionTree);
		Move convertToPhysicalMove(Move logicalMove);

		//debug function
		void displayBoard();

		//update qValue
		void feedbackQValue(uint8_t trueAction);
	public:
		//number of move to look forward
		uint8_t depth;

		//you have to define this->player yourself
		GomokuAI(GomokuPVE* gomokuPVE, uint8_t depth);
		~GomokuAI(void);

		void takeTurn(void);
		void getReady(Player myPlayer);
		Player getPlayer(void);
	};
}
