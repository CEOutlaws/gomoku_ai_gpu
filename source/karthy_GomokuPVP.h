#pragma once

#include "karthy_Common.h"
#include "karthy_Gomokugame.h"

using namespace cv;

namespace karthy
{
	class GomokuPVP : public GomokuGame
	{
	public:
		const int stoneRadius = 15;
		Mat gameFrame;

		//init the game's rule
		GomokuPVP(); //default 19x19, 5 in a rows
		GomokuPVP(int boardCols, uint8_t stonesToWin);
		~GomokuPVP();
		
		//handle mouse input, (x, y) is the coordinate which the mouse clicked on
		virtual void MouseHandler(int event, int x, int y);
		static void MouseHandler(int event, int x, int y, int flags, void* userdata);
		//get which object did the mouse point to
		bool isPointToReplayIcon(Point2i mouseCoor);
		bool isPointToBoard(Point2i mouseCoor);

		void run(void);
		virtual void replay(void);
		void initGUI(void);
		void loadMap(void);
		void showMap(void);

		//get Index of the box which the mouse click on
		Index GetBoxIndex(Point2i mouseCoor);

		virtual void executeMove(Move move);
		void handleMoveConsequence(void);

		void writeText(Text text);
		void eraseText(Text text);
	};
}

