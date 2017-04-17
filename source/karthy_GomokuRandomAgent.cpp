#include "karthy_GomokuRandomAgent.h"

using namespace karthy;

GomokuRandomAgent::GomokuRandomAgent(GomokuPVE* gomokuGame)
{
	this->myPhysicalGame = gomokuGame;
	this->emptyBoxCount = 0;
	this->previousRandomMove = Point2i(-1,-1);
}


GomokuRandomAgent::~GomokuRandomAgent()
{
}
Move karthy::GomokuRandomAgent::takeTurn(void)
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
/*
Move karthy::GomokuRandomAgent::takeTurn(void)
{
	Move result = NULL_MOVE;

	std::random_device randomDevice;  //Will be used to obtain a seed for the random number engine
	std::mt19937 generator(randomDevice()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distributor(0, this->emptyBoxCount - 1);

	int selectedBoxOrder = distributor(generator);
	int emptyBoxOrder = 0;

	GomokuBoard& board = this->myPhysicalGame->board;

	for (uint8_t boxIndexX = 0; boxIndexX < board.colCount; boxIndexX++)
	{
		for (uint8_t boxIndexY = 0; boxIndexY < board.rowCount; boxIndexY++)
		{
			Index boxIndex = Index(boxIndexX, boxIndexY);
			if (board.getBoxStatus(boxIndex) == BoxStatus::HAVE_NO_STONE)
			{
				if (selectedBoxOrder == emptyBoxOrder)
				{
					result = (Move)boxIndex;
					break;
				}
				emptyBoxOrder++;
			}
		}
		if (result != NULL_MOVE) { break; }
	}

	this->emptyBoxCount -= 2;
	return result;
}
*/
void karthy::GomokuRandomAgent::getReady(Player myPlayer)
{
	this->myPlayer = myPlayer;
	this->emptyBoxCount = this->myPhysicalGame->board.colCount*this->myPhysicalGame->board.rowCount - (uint16_t)myPlayer;
}
