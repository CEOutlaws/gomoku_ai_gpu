#pragma once
#include <forward_list> 
#include "karthy_nodedata.h"
#include "karthy_common.h"
#include "karthy_edge.h"

using namespace std;

namespace karthy 
{
	class Edge;
	class Node
	{
	private:
		//To be called in every class's constructor
		void initNode(NodeData* initNodeData);

	public:
		NodeData* data;
		forward_list<Edge*>* edgeList;

		Node(uint64 id, uint8_t type);
		Node(NodeData* data);
		~Node();

		uint64_t getId();
		void addEdge(Edge* newEdge);
		//void DeleteAllChildExcept(node_c* exceptChild);
		//Delete all edge in the childList, but don't delete the childList
		void deleteEdgeList(void);
		void deleteAllEdge(void);
	};
}

