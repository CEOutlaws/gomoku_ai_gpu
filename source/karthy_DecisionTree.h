#pragma once
#include "karthy_Tree.h"
namespace karthy
{
	typedef Node State;
	typedef Edge Action;

	class DecisionTree : public Tree
	{
	public:
		uint16_t actionCount;
		bool symmetric;

		DecisionTree();
		~DecisionTree();

		void clear();
	};
}

