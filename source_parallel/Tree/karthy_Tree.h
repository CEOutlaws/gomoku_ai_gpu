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

		//setNull the content in Tree
		virtual void setNull();
	};
}

