#pragma once
#include "karthy_common.h"

namespace karthy
{
	class NodeData
	{
	public:
		uint64_t id;
		uint8_t type;

		NodeData(uint64_t id, uint8_t type);
	};
}

