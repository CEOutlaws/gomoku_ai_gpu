#include "karthy_DecisionTree.h"

using namespace karthy;

karthy::DecisionTree::DecisionTree()
{
	this->actionCount = 0;
	this->symmetric = false;
}


karthy::DecisionTree::~DecisionTree()
{
}

void karthy::DecisionTree::clear()
{
	Tree::clear();
	this->actionCount = 0;
	this->symmetric = false;
}
