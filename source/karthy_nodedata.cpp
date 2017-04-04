#include "karthy_nodedata.h"

using namespace karthy;


karthy::NodeData::NodeData(int initX, int initY, uint8_t initType, double initQ)
{
	this->x = initX;
	this->y = initY;
	this->type = initType;
	this->Q = initQ;
}