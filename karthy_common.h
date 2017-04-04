#pragma once
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define BLACK_COLOR { 0, 0, 0 }
#define WHITE_COLOR { 255, 255, 255 }
#define BACKGROUND_COLOR { 0x66, 0x99, 0xCC }

using namespace cv;

namespace karthy
{
	enum class gamestatus_c : char
	{
		ENDED = 0,
		PLAYING = 1
	};

	enum class boxstatus_c : char
	{
		HAVE_BLACK_STONE = 0,
		HAVE_WHITE_STONE = 1,
		HAVE_NO_STONE = 2,
	};

	enum class player_c : char
	{
		BLACK_PLAYER = 0,
		WHITE_PLAYER = 1,
		NO_PLAYER = 2
	};

	typedef Point2i index;
	typedef Point2i gamemove;
}