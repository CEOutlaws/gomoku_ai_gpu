#include "karthy_tree.h"

using namespace karthy;

karthy::Tree::Tree()
{
	this->root = NULL;
}


karthy::Tree::~Tree()
{
	this->clear();
}

void karthy::Tree::clear(void)
{
	if (root == NULL)
	{
		return;
	}
	delete root;
	root = NULL;
}