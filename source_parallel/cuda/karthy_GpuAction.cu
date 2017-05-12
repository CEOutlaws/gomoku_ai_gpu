#include "karthy_GpuAction.h"

/*
karthy::cuda::GpuAction::GpuAction()
{
}
*/
karthy::cuda::GpuAction::GpuAction(int x, int y)
{
	this->x = x;
	this->y = y;
}

void karthy::cuda::GpuAction::operator=(const GpuAction &rhsAction)
{
	this->x = rhsAction.x;
	this->y = rhsAction.y;
	this->qValue = rhsAction.qValue;
	this->order = rhsAction.order;
	this->nextStateId = rhsAction.nextStateId;
}

void karthy::cuda::GpuAction::setNull(void)
{
	this->x = -1;
}

bool karthy::cuda::GpuAction::isNull(void)
{
	return (this->x == -1);
}
