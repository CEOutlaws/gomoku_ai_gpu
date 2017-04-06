#pragma once
#include "karthy_common.h"
#include "karthy_tree.h"
#include "karthy_node.h"
#include "karthy_nodedata.h"
#include "karthy_gomokuPVE.h"

namespace karthy
{
	class GomokuPVE;
	class GomokuAI
	{
		typedef Tree DecisionTree;
		typedef Node DecisionNode;

	private:
		class Score
		{
		public:
			const double WIN = 1000.0;
			const double LOSE = -1000.0;
			const double DRAW = 0.0;
			const double DEFAULT = 0.0;
		} Score;
		class Evalute
		{
		public:
			double grammar = 0.9;
			double anpha = 0.9;
		}Evalute;
		GomokuPVE* _game;
		DecisionTree decisionTree;
		//build DecisionTree from root (the current game board state)
		void buildDecisionTree(void);
		//clear the content in DecisionTree
		void clearDecisionTree(void);
		//add availble next state node (available child node)
		void addAvailableNextNode(DecisionNode* toState, uint8_t depth);
		//check symmetric for reduction
		bool isSymmetric(forward_list<Node*>* currentNextNodeList, Move nextMoveToCheck, BoxStatus newBoxStatus);
		//calculate Q match
		void estimateNode(Node* currentNode, double reward);
		void estimateDecisionTree(void);
		//select best Q
		DecisionNode* selectBestQ(void);
	public:
		//AI's playing role
		Player player; 
		//number of move to look forward
		uint8_t depth;	 

		//you have to define this->player yourself
		GomokuAI(GomokuPVE* gomokuPVE, uint8_t depth);
		~GomokuAI();

		void takeTurn(void);
	};
}
