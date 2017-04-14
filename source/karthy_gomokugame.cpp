#include "karthy_GomokuGame.h"

using namespace karthy;

karthy::GomokuGame::GomokuGame(int boardCols, uint8_t stonesToWin)
{
	board.colCount = boardCols;
	board.sideLength = board.colCount * board.boxParam.sideLength;
	_stonesToWin = stonesToWin;

	board.initBoxStatus(board.colCount, board.rowCount);
}


karthy::GomokuGame::~GomokuGame()
{
}

void karthy::GomokuGame::newGame(void)
{
	this->activePlayer = Player::BLACK_PLAYER;
	this->latestMove = NULL_MOVE;
	this->gameStatus = GameStatus::PLAYING;
	this->board.setAllBoxStatus(BoxStatus::HAVE_NO_STONE);
}

void karthy::GomokuGame::switchPlayer(void)
{
	this->activePlayer = !activePlayer;
}

uint8_t karthy::GomokuGame::getStoneToWin(void)
{
	return this->_stonesToWin;
}

Player karthy::GomokuGame::getWinner(void)
{
	Player winner = Player::NO_PLAYER;
	if (this->latestMove == NULL_MOVE) { return winner; }

	if (this->isVerticalEnd() || this->isHorizontalEnd() || 
		this->isDiagonalRLEnd() || this->isDiagonalLREnd())
	{
		winner = (Player)(board.getBoxStatus(this->latestMove));
	}

	return winner;
}

void karthy::GomokuGame::executeMove(Move move)
{
	this->latestMove = move;
	this->board.setBoxStatus(move, (BoxStatus)this->activePlayer);
	this->switchPlayer();
}

bool karthy::GomokuGame::isVerticalEnd(void)
{
	char stoneCount = 1;

	for (int boxIndexY = latestMove.y + 1; boxIndexY < board.rowCount; boxIndexY++)
	{
		if (board.getBoxStatus(latestMove) != board.getBoxStatus(Index(latestMove.x, boxIndexY)))
		{
			break;
		}
		else
		{
			stoneCount++;
		}
	}

	for (int boxIndexY = latestMove.y - 1; boxIndexY >= 0; boxIndexY--)
	{
		if (board.getBoxStatus(latestMove) != board.getBoxStatus(Index(latestMove.x, boxIndexY)))
		{
			break;
		}
		else
		{
			stoneCount++;
		}
	}
	return (stoneCount >= _stonesToWin);
}

bool karthy::GomokuGame::isHorizontalEnd(void)
{
	char stoneCount = 1;

	for (int boxIndexX = latestMove.x + 1; boxIndexX < board.colCount; boxIndexX++)
	{
		BoxStatus a = board.getBoxStatus(Index(boxIndexX, latestMove.y));
		if (board.getBoxStatus(latestMove) != board.getBoxStatus(Index(boxIndexX, latestMove.y)))
		{
			break;
		}
		else
		{
			stoneCount++;
		}
	}

	for (int boxIndexX = latestMove.x - 1; boxIndexX >= 0; boxIndexX--)
	{
		BoxStatus a = board.getBoxStatus(Index(boxIndexX, latestMove.y));
		BoxStatus b = board.getBoxStatus(latestMove);
		if (board.getBoxStatus(latestMove) != board.getBoxStatus(Index(boxIndexX, latestMove.y)))
		{
			break;
		}
		else
		{
			stoneCount++;
		}
	}
	return (stoneCount >= _stonesToWin);
}

bool karthy::GomokuGame::isDiagonalLREnd(void)
{
	char stoneCount = 1;

	for (
		int boxIndexX = latestMove.x + 1, boxIndexY = latestMove.y + 1;
		boxIndexX < board.colCount && boxIndexY < board.rowCount;
		boxIndexX++, boxIndexY++
		)
	{
		if (board.getBoxStatus(latestMove) != board.getBoxStatus(Index(boxIndexX, boxIndexY)))
		{
			break;
		}
		else
		{
			stoneCount++;
		}
	}

	for (
		int boxIndexX = latestMove.x - 1, boxIndexY = latestMove.y - 1;
		boxIndexX >= 0 && boxIndexY >= 0;
		boxIndexX--, boxIndexY--
		)
	{
		if (board.getBoxStatus(latestMove) != board.getBoxStatus(Index(boxIndexX, boxIndexY)))
		{
			break;
		}
		else
		{
			stoneCount++;
		}
	}
	return (stoneCount >= _stonesToWin);
}

bool karthy::GomokuGame::isDiagonalRLEnd(void)
{
	char stoneCount = 1;

	for (
		int boxIndexX = latestMove.x + 1, boxIndexY = latestMove.y - 1;
		boxIndexX < board.colCount && boxIndexY >= 0;
		boxIndexX++, boxIndexY--
		)
	{
		if (board.getBoxStatus(latestMove) != board.getBoxStatus(Index(boxIndexX, boxIndexY)))
		{
			break;
		}
		else
		{
			stoneCount++;
		}
	}

	for (
		int boxIndexX = latestMove.x - 1, boxIndexY = latestMove.y + 1;
		boxIndexX >= 0 && boxIndexY < board.rowCount;
		boxIndexX--, boxIndexY++
		)
	{
		if (board.getBoxStatus(latestMove) != board.getBoxStatus(Index(boxIndexX, boxIndexY)))
		{
			break;
		}
		else
		{
			stoneCount++;
		}
	}
	return (stoneCount >= _stonesToWin);
}