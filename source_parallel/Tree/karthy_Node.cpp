#include "karthy_Node.h"

karthy::Node::Node(uint64 id, uint8_t type)
{
	NodeData* newNodeData = new NodeData(id, type);
	initNode(newNodeData);
}

karthy::Node::Node(NodeData* data)
{
	initNode(data);
}

karthy::Node::~Node()
{
	//delete data
	delete this->data;
	//this->data = NULL;

	if (this->edgeList == NULL) { return; }
	//delete childList
	this->deleteEdgeList();
	//this->childList = NULL;
}

uint64_t karthy::Node::getId()
{
	return this->data->id;
}

void karthy::Node::initNode(NodeData* initNodeData)
{
	this->data = initNodeData;
	this->edgeList = NULL;
}

void karthy::Node::deleteAllEdge(void)
{
	for (; !(this->edgeList->empty()); this->edgeList->pop_front())
	{
		delete this->edgeList->front();
	}
}

void karthy::Node::addEdge(Edge* newEdge)
{
	if (this->edgeList == NULL)
	{
		this->edgeList = new forward_list<Edge*>;
	}

	this->edgeList->push_front(newEdge);
}

void karthy::Node::deleteEdgeList(void)
{
	deleteAllEdge();
	delete this->edgeList;
}
