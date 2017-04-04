#pragma once
#include "karthy_node.h"

namespace karthy
{
	class Tree
	{
	public:
		Node* root;

		Tree();
		~Tree();

		//clear the content in Tree
		void clear();
	};
}

