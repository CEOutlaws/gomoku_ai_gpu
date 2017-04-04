#include "karthy_node.h"

using namespace karthy;

karthy::Node::Node(int initX, int initY, uint8_t initType, double initQ)
{
	NodeData* newNodeData = new NodeData(initX, initY, initType, initQ);
	initNode(newNodeData);
}

karthy::Node::Node(NodeData* initNodeData)
{
	initNode(initNodeData);
}

karthy::Node::~Node()
{
	//delete data
	delete this->data;
	//this->data = NULL;

	if (this->childList == NULL) { return; }
	//delete childList
	this->deleteChildList();
	//this->childList = NULL;
}

void karthy::Node::initNode(NodeData* initNodeData)
{
	this->data = initNodeData;
	this->childList = NULL;
}

void karthy::Node::deleteAllChild(void)
{
	for (; !(this->childList->empty()); this->childList->pop_front())
	{
		delete this->childList->front();
	}
}

void karthy::Node::addChild(Node* newChild)
{
	if (childList == NULL)
	{
		childList = new forward_list<Node*>;
	}

	childList->push_front(newChild);
}

void karthy::Node::deleteChildList(void)
{
	deleteAllChild();
	delete this->childList;
}
