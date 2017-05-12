#include "karthy_Tree.h"

karthy::Tree::Tree()
{
	this->root = NULL;
}


karthy::Tree::~Tree()
{
	this->setNull();
}

void karthy::Tree::setNull(void)
{
	if (root == NULL)
	{
		return;
	}

	delete root;
	root = NULL;
}