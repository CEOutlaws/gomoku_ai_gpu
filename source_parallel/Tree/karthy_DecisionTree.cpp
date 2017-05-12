#include "karthy_DecisionTree.h"

karthy::DecisionTree::DecisionTree()
{
	this->actionCount = 0;
	this->symmetric = false;
}


karthy::DecisionTree::~DecisionTree()
{
}

void karthy::DecisionTree::setNull()
{
	Tree::setNull();
	this->actionCount = 0;
	this->symmetric = false;
}
