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
		//Gomoku is played on a square board
		struct gomokuboard_t
		{
			//There are boxes on the board
			struct gomokubox_t
			{
				const int sideLength = 37;
				const int marginThickness = 2;	//margin cover around the box

				boxstatus_c status[19][19];	//each box have its status: WHITE/BLACK/NO STONE IS PLACED on the box
			} box;

			int cols; //number of collumn
			int sideLength; //length

			const int offset = 7;
			const int maxCols = 19; //game support upto 19x19 board
			const int maxSideLength = maxCols * box.sideLength;
		} board;

		const int stoneRadius = 15;

		bool activePlayer;	//player in turn
		Mat gameFrame;
		gamestatus_c gameStatus; //PLAYING or Ended
		gamemove latestMove;

		//init the game's rule
		gomokuPVP_c(); //default 19x19, 5 in a rows
		gomokuPVP_c(int boardCols, char stonesToWin);
		~gomokuPVP_c();
		
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

		//each box have its status: WHITE/BLACK/NO STONE IS PLACED on the box
		boxstatus_c GetBoxStatus(index boxIndex);
		void SetBoxStatus(index boxIndex, boxstatus_c newBoxStatus);
		void SetAllBoxStatus(boxstatus_c newBoxStatus);

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

