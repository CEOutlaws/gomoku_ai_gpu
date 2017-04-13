#pragma once
#include "karthy_Common.h"
#include "karthy_Tree.h"
#include "karthy_Node.h"
#include "karthy_Nodedata.h"
#include "karthy_Edge.h"
#include "karthy_GomokuPVE.h"
#include "karthy_AiInfo.h"

namespace karthy
{
	typedef Tree DecisionTree;
	typedef Node State;
	typedef Edge Action;

	class GomokuPVE;
	class GomokuAgent
	{	
	protected:
		GomokuPVE* _game;
		DecisionTree decisionTree;

		//agent's playing role
		Player myPlayer;

		//build DecisionTree from root (the current game board state)
		virtual void buildDecisionTree(void) = 0;

		//clear the content in DecisionTree
		virtual void clearDecisionTree(void) = 0;

		//add availble next state node (available child node)
		virtual void addAvailableAction(State* toState, uint8_t depth) = 0;

		//given decision tree, select next action
		virtual Action* selectAction(DecisionTree& decisionTree) = 0;
	public:
		virtual Move takeTurn(void) = 0;
		virtual void getReady(Player myPlayer) = 0;
		virtual Player getPlayer(void) = 0;
	};
}

