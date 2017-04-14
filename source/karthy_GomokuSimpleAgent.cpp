#include "karthy_GomokuSimpleAgent.h"

using namespace karthy;

karthy::GomokuSimpleAgent::GomokuSimpleAgent(GomokuPVE* gomokuGame)
{
	this->myPhysicalGame = gomokuGame;
}


karthy::GomokuSimpleAgent::~GomokuSimpleAgent()
{

}

Move karthy::GomokuSimpleAgent::takeTurn(void)
{
	Move result = NULL_MOVE;

	GomokuBoard& board = this->myPhysicalGame->board;
	for (uint8_t boxIndexX = 0; boxIndexX < board.colCount && result == NULL_MOVE; boxIndexX++)
	{
		for (uint8_t boxIndexY = 0; boxIndexY < board.rowCount && result == NULL_MOVE; boxIndexY++)
		{
			Index boxIndex = Index(boxIndexX, boxIndexY);
			if (board.getBoxStatus(boxIndex) == BoxStatus::HAVE_NO_STONE)
			{
				result = (Move)boxIndex;
			}
		}
	}

	return result;
}

void karthy::GomokuSimpleAgent::getReady(Player myPlayer)
{
	this->myPlayer = myPlayer;
}
