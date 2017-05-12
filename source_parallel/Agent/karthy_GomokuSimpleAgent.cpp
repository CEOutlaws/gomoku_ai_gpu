#include "karthy_GomokuSimpleAgent.h"

karthy::GomokuSimpleAgent::GomokuSimpleAgent(GomokuPVE* gomokuGame)
{
	this->myPhysicalGame = gomokuGame;
}


karthy::GomokuSimpleAgent::~GomokuSimpleAgent()
{

}

karthy::Move karthy::GomokuSimpleAgent::takeTurn(void)
{
	Move result = NULL_MOVE;

	GomokuBoard& board = this->myPhysicalGame->board;

	for (int boxIndexY = 0; boxIndexY < board.colCount && result == NULL_MOVE; boxIndexY++)
	{
		for (int boxIndexX = 0; boxIndexX < board.colCount && result == NULL_MOVE; boxIndexX++)
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
