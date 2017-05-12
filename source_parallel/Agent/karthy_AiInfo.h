#pragma once
#include "..\karthy_Common.h"

namespace karthy
{
	class AiInfo
	{
	public:
		uint64_t stateCount;
		std::string infoPath;

		AiInfo(std::string infoPath);
		~AiInfo();

		void load(void);
		void save(void);
	};
}
