#pragma once
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <string>

#define BLACK_COLOR { 0, 0, 0 }
#define WHITE_COLOR { 255, 255, 255 }
#define GRAY_COLOR { 127, 127, 127 }
#define BLUE_COLOR { 255, 0, 0 }
#define BACKGROUND_COLOR { 0x66, 0x99, 0xCC }
#define NULL_MOVE (Move(-1, -1))

#define WINDOW_NAME "Karthy Gomoku"
#define KARTHY_MEMORY_PATH "D:\\karthy_memory\\"
//".." is the folder where file.sln is located
//#define KARTHY_MEMORY_PATH "../karthy_memory/"
#define BACK_GROUND_PATH "D:\\background\\caro_background.jpg"
//#define BACK_GROUND_PATH "../background/caro_background.jpg"
#define DATABASE_ANALYZER_PATH "D:\\karthy_analyzer\\analyzer.csv"

using namespace cv;

namespace karthy
{
	//class GomokuPVP;
	enum class Text : uint8_t
	{
		THINKING,
		DRAW,
		BLACK_WIN,
		WHITE_WIN
	};

	enum class GameStatus : bool
	{
		ENDED = 0,
		PLAYING = 1
	};

	enum class BoxStatus : uint8_t
	{
		HAVE_BLACK_STONE = 0,
		HAVE_WHITE_STONE = 1,
		HAVE_NO_STONE = 2,
	};
	BoxStatus operator!(BoxStatus &operand);

	enum class Player : uint8_t
	{
		BLACK_PLAYER = 0,
		WHITE_PLAYER = 1,
		NO_PLAYER = 2
	};
	Player operator!(Player &operand);

	typedef Point2i Index;
	typedef Point2i Move;

	void displayBoard(void);
	void printBoard(Mat board);
}