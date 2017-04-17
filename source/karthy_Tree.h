#pragma once
#include "karthy_Node.h"

namespace karthy
{
	class Tree
	{
	public:
		Node* root;

		Tree();
		~Tree();

		//clear the content in Tree
		virtual void clear();
	};
}

