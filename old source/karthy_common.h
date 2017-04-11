#pragma once
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define BLACK_COLOR { 0, 0, 0 }
#define WHITE_COLOR { 255, 255, 255 }
#define GRAY_COLOR { 127, 127, 127 }
#define BACKGROUND_COLOR { 0x66, 0x99, 0xCC }
#define NULL_MOVE (Move(-1, -1))
using namespace cv;

namespace karthy
{
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

	typedef Point_<char> Point2c;
	typedef Point2i Index;
	typedef Point2i Move;


}