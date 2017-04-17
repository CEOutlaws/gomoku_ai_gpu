#include "karthy_GomokuNormalRandomAgent.h"

using namespace karthy;

karthy::GomokuNormalRandomAgent::GomokuNormalRandomAgent(GomokuPVE* gomokuGame)
{
	this->myPhysicalGame = gomokuGame;
	this->emptyBoxCount = 0;
	this->previousRandomMove = Point2i(-1,-1);
}


karthy::GomokuNormalRandomAgent::~GomokuNormalRandomAgent()
{
}

Move karthy::GomokuNormalRandomAgent::takeTurn(void)
{
	Move result = NULL_MOVE;
	if (this->previousRandomMove == Point(-1, -1))
	{
		std::random_device randomDevice;  //Will be used to obtain a seed for the random number engine
		std::mt19937 generator(randomDevice()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> distributorX(0, this->myPhysicalGame->board.boxStatus.cols - 1);
		std::uniform_int_distribution<> distributorY(0, this->myPhysicalGame->board.boxStatus.cols - 1);
		result.x = (int)distributorX(generator);
		result.y = (int)distributorY(generator);
		while (this->myPhysicalGame->board.getBoxStatus(result) != BoxStatus::HAVE_NO_STONE)
		{
			result.x = (int)distributorX(generator);
			result.y = (int)distributorY(generator);
		}
	}
	else
	{
		std::random_device randomDevice;  //Will be used to obtain a seed for the random number engine
		std::mt19937 generator(randomDevice()); //Standard mersenne_twister_engine seeded with rd()
		std::normal_distribution<> distributorX(this->previousRandomMove.x, STD_DEV);
		result.x = (int)distributorX(generator);
		if (result.x < 0) 
		{
			result.x = 0;
		}
		if (result.x >= this->myPhysicalGame->board.boxStatus.cols)
		{
			result.x = this->myPhysicalGame->board.boxStatus.cols-1;
		}
		std::normal_distribution<> distributorY(this->previousRandomMove.y, STD_DEV);
		result.y = (int)distributorY(generator);
		if (result.y < 0)
		{
			result.y = 0;
		}
		if (result.y >= this->myPhysicalGame->board.boxStatus.cols)
		{
			result.y = this->myPhysicalGame->board.boxStatus.cols-1;
		}
		while (this->myPhysicalGame->board.getBoxStatus(result) != BoxStatus::HAVE_NO_STONE)
		{
			result.x = (int)distributorX(generator);
			if (result.x < 0)
			{
				result.x = 0;
			}
			if (result.x >= this->myPhysicalGame->board.boxStatus.cols)
			{
				result.x = this->myPhysicalGame->board.boxStatus.cols-1;
			}

			result.y = (int)distributorY(generator);
			if (result.y < 0)
			{
				result.y = 0;
			}
			if (result.y >= this->myPhysicalGame->board.boxStatus.cols)
			{
				result.y = this->myPhysicalGame->board.boxStatus.cols-1;
			}
		}	
	}
	this->previousRandomMove = result;
	return result;
}

void karthy::GomokuNormalRandomAgent::getReady(Player myPlayer)
{
	this->myPlayer = myPlayer;
	this->emptyBoxCount = this->myPhysicalGame->board.colCount*this->myPhysicalGame->board.rowCount - (uint16_t)myPlayer;
}
