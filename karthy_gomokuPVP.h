#pragma once

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "karthy_common.h"

using namespace cv;

namespace karthy
{
	class gomokuPVP_c
	{
	private:
		char _stonesToWin;
	public:

		//init the game's rule
		gomokuPVP_c(); //default 19x19, 5 in a rows
		gomokuPVP_c(int boardCols, char stonesToWin);
		~gomokuPVP_c();

		//Gomoku is played on a square board
		class gomokuboard_c 
		{
		public:
			//There are boxes on the board
			//box's parameter
			struct
			{
				const int sideLength = 37;
				const int marginThickness = 2;	//margin cover around the box
			} boxParam;
			//box's status
			//each box have its status: WHITE/BLACK/NO STONE IS PLACED on the box
			boxstatus_c** boxStatus;

			boxstatus_c GetBoxStatus(index boxIndex);
			void SetBoxStatus(index boxIndex, boxstatus_c newBoxStatus);
			void SetAllBoxStatus(boxstatus_c newBoxStatus);

			//board's parameter
			int colCount; //number of collumn
			int &rowCount = colCount; //number of rows, because cols = rows so I use refference
			int sideLength; //length

			const int offset = 7;
			const int maxCols = 19; //game support upto 19x19 board
			const int maxSideLength = maxCols * boxParam.sideLength;
		} board;

		const int stoneRadius = 15;

		bool activePlayer;	//player in turn
		Mat gameFrame;
		gamestatus_c gameStatus; //PLAYING or Ended
		gamemove latestMove;

		//handle mouse input, (x, y) is the coordinate which the mouse clicked on
		void MouseHandler(int event, int x, int y);
		static void MouseHandler(int event, int x, int y, int flags, void* userdata);
		//get which object did the mouse point to
		bool IsPointToReplayIcon(Point2i mouseCoor);
		bool IsPointToBoard(Point2i mouseCoor);

		void Run(void);
		void NewGame(void);
		void Replay(void);
		void GUIInit(void);
		void LoadMap(void);

		//get index of the box which the mouse click on
		Point2i GetBoxIndex(Point2i mouseCoor);

		void ExecuteMove(index boxIndex);
		void HandleMoveConsequence(void);
		player_c GetWinner(void); //WHITE/BLACK/NO ONE WIN
		void SwitchPlayer(void);

		//check if game ended
		bool IsVerticalEnd(void);
		bool IsHorizontalEnd(void);
		bool IsDiagonalLREnd(void);
		bool IsDiagonalRLEnd(void);
	};
}

