#pragma once
#include "karthy_common.h"

namespace karthy
{
	class NodeData
	{
	public:
		NodeData(int x, int y, uint8_t type, double Q);

		int x;
		int y;
		/*
		*type 0:
		*type 1: 
		*type 2: end game terminated
		*type 3: estimated terminated
		*/
		uint8_t type;
		double Q;
	};
}

