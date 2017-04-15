#include "karthy_AiInfo.h"

using namespace karthy;

karthy::AiInfo::AiInfo()
{
	this->stateCount = 0;
}


karthy::AiInfo::~AiInfo()
{
}

void karthy::AiInfo::load(void)
{
	std::ifstream infoFile;
	infoFile.open((std::string)KARTHY_MEMORY_PATH + "info");

	if (!infoFile.is_open())
	{
		this->stateCount = 0;
	}
	else
	{
		infoFile >> this->stateCount;
		infoFile.close();
	}
}

void karthy::AiInfo::save(void)
{
	std::ofstream infoFile((std::string)KARTHY_MEMORY_PATH + "info");
	infoFile << this->stateCount;
	infoFile.close();
}

