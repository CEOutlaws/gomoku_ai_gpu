#include "karthy_Edge.h"

using namespace karthy;

karthy::Edge::Edge(int x, int y, double qValue, Node* otherNode)
{
	this->x = x;
	this->y = y;
	this->qValue = qValue;
	this->otherNode = otherNode;
}


karthy::Edge::~Edge()
{
	delete this->otherNode;
}
