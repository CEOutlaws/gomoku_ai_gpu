#pragma once
#include "karthy_node.h"

namespace karthy
{
	class Node;
	class Edge
	{
	public:
		int x;
		int y;
		/*
		*type 0:
		*type 1:
		*type 2: end game terminated
		*type 3: estimated terminated
		*/
		double qValue;
		Node* otherNode;

		Edge(int x, int y, double qValue, Node* otherNode);
		~Edge();
	};
}

