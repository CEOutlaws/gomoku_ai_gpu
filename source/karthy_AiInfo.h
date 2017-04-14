#pragma once
#include "karthy_common.h"

namespace karthy
{
	class AiInfo
	{
	public:
		uint64_t stateCount;

		AiInfo();
		~AiInfo();

		void load(void);
		void save(void);
	};
}
