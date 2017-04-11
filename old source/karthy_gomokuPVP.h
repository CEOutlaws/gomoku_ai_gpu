#pragma once

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "karthy_common.h"

using namespace cv;

namespace karthy
{
	class GomokuPVP
	{
	private:
		char _stonesToWin;
	public:

		//init the game's rule
		GomokuPVP(); //default 19x19, 5 in a rows
		GomokuPVP(int boardCols, char stonesToWin);
		~GomokuPVP();

		//Gomoku is played on a square board
		class GomokuBoard 
		{
		public:
			//////There are boxes on the board/////

			//box's parameter
			struct
			{
				const int sideLength = 37;
				const int marginThickness = 2;	//margin cover around the box
			} boxParam;

			///box's status
			//each box have its status: WHITE/BLACK/NO STONE IS PLACED on the box
			Mat boxStatus;

			BoxStatus getBoxStatus(Index boxIndex);
			void setBoxStatus(Index boxIndex, BoxStatus newBoxStatus);
			void setAllBoxStatus(BoxStatus newBoxStatus);
			void initBoxStatus(int colCount, int rowCount);
			bool isFullBox(void);

			///////////board's parameter////////////

			//number of collumn
			int colCount; 
			//number of rows, because cols = rows so I use refference
			int &rowCount = colCount; 
			//length
			int sideLength;
			const int offset = 7;
			//game support upto 19x19 board
			const int maxCols = 19; 
			const int maxSideLength = maxCols * boxParam.sideLength;
			///////////////////////////////////////
		} board;

		const int stoneRadius = 15;

		Player activePlayer;	//player in turn
		Mat gameFrame;
		GameStatus gameStatus; //PLAYING or Ended
		Move latestMove;

		//handle mouse input, (x, y) is the coordinate which the mouse clicked on
		virtual void MouseHandler(int event, int x, int y);
		static void MouseHandler(int event, int x, int y, int flags, void* userdata);
		//get which object did the mouse point to
		bool isPointToReplayIcon(Point2i mouseCoor);
		bool isPointToBoard(Point2i mouseCoor);

		void run(void);
		virtual void newGame(void);
		virtual void Replay(void);
		void initGUI(void);
		void loadMap(void);
		void showMap(void);

		//get Index of the box which the mouse click on
		Index GetBoxIndex(Point2i mouseCoor);

		void executeMove(Index boxIndex);
		void handleMoveConsequence(void);
		Player getWinner(void); //WHITE/BLACK/NO ONE WIN
		void switchPlayer(void);

		//check if game ended
		bool isVerticalEnd(void);
		bool isHorizontalEnd(void);
		bool isDiagonalLREnd(void);
		bool isDiagonalRLEnd(void);
	};
}

