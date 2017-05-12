#include "karthy_AiInfo.h"

karthy::AiInfo::AiInfo(std::string infoPath)
{
	this->infoPath = infoPath;
	this->stateCount = 0;
}


karthy::AiInfo::~AiInfo()
{
}

void karthy::AiInfo::load(void)
{
	std::ifstream infoFile;
	infoFile.open(infoPath + "info");

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
	std::ofstream infoFile(infoPath + "info");
	infoFile << this->stateCount;
	infoFile.close();
}

