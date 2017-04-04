#pragma once
#include <forward_list> 
#include "karthy_nodedata.h"
#include "karthy_common.h"

using namespace std;

namespace karthy 
{
	class Node
	{
	private:
		//To be called in every class's constructor
		void initNode(NodeData* initNodeData);

	public:
		NodeData* data;
		forward_list<Node*>* childList;

		Node(int x, int y, uint8_t type, double Q);
		Node(NodeData* initNodeData);
		~Node();

		void addChild(Node* newChild);
		//void DeleteAllChildExcept(node_c* exceptChild);
		//Delete all child in the childList, but don't delete the childList
		void deleteChildList(void);
		void deleteAllChild(void);
	};
}

