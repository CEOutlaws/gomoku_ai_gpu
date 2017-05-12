#pragma once
#include "..\karthy_Common.h"
#include "karthy_GomokuBoard.h"

namespace karthy
{
	class GomokuGame
	{
	private:
		uint8_t _stonesToWin;

		//check if game ended
		bool isVerticalEnd(void);
		bool isHorizontalEnd(void);
		bool isDiagonalLREnd(void);
		bool isDiagonalRLEnd(void);

	public:
		//player in turn
		Player activePlayer;
		//PLAYING or Ended
		GameStatus gameStatus;
		Move latestMove;
		//Gomoku is played on a square board
		GomokuBoard board;

		GomokuGame();
		GomokuGame(int boardCols, uint8_t stonesToWin);
		~GomokuGame();

		virtual void newGame(void);
		virtual void executeMove(Move move);

		Player getWinner(void); //WHITE/BLACK/NO ONE WIN
		void switchPlayer(void);
		uint8_t getStoneToWin(void);
	};
}

