#include "karthy_GomokuPVP.h"

karthy::GomokuPVP::GomokuPVP()
{
}

karthy::GomokuPVP::GomokuPVP(int boardCols, uint8_t stonesToWin) : GomokuGame(boardCols, stonesToWin)
{
}

karthy::GomokuPVP::~GomokuPVP()
{
}

void karthy::GomokuPVP::MouseHandler(int event, int x, int y)
{
	if (event == EVENT_LBUTTONDOWN ||
		event == EVENT_RBUTTONDOWN ||
		event == EVENT_MBUTTONDOWN)
	{
		//if in Replay Icon
		const Point2i mouseCoor(x, y);
		if (isPointToReplayIcon(mouseCoor))
		{
			replay();
		}
		else if (gameStatus == GameStatus::PLAYING && isPointToBoard(mouseCoor))
		{
			Index pointedBoxIndex = GetBoxIndex(mouseCoor);

			if (board.getBoxStatus(pointedBoxIndex) == BoxStatus::HAVE_NO_STONE)
			{
				executeMove(pointedBoxIndex);
			}
		}
	}
}

void karthy::GomokuPVP::MouseHandler(int event, int x, int y, int flags, void* userdata)
{
	GomokuPVP* pGomokuPVP_c = reinterpret_cast<GomokuPVP*>(userdata);
	pGomokuPVP_c->MouseHandler(event, x, y);
}

void karthy::GomokuPVP::loadMap()
{
	gameFrame = imread(BACK_GROUND_PATH, 1);
	for (int pixelX = board.sideLength; pixelX < board.maxSideLength; pixelX++)
	{
		for (int pixelY = 0; pixelY < board.maxSideLength + board.boxParam.marginThickness; pixelY++)
		{
			gameFrame.at<Vec3b>(Point(pixelX + board.offset + board.boxParam.marginThickness, pixelY + board.offset)) = BACKGROUND_COLOR;
		}
	}
	for (int pixelX = 0; pixelX < board.sideLength + board.boxParam.marginThickness; pixelX++)
	{
		for (int pixelY = board.sideLength; pixelY < board.maxSideLength; pixelY++)
		{
			gameFrame.at<Vec3b>(Point(pixelX + board.offset, pixelY + board.offset + board.boxParam.marginThickness)) = BACKGROUND_COLOR;
		}
	}

	showMap();
}

void karthy::GomokuPVP::showMap(void)
{
	imshow(WINDOW_NAME, gameFrame);
	waitKey(1);
}
void karthy::GomokuPVP::replay(void)
{
	loadMap();
	newGame();
}

void karthy::GomokuPVP::initGUI(void)
{
	loadMap();
	//setNull the callback function for any mouse event
	setMouseCallback(WINDOW_NAME, MouseHandler, this);
}

karthy::Index karthy::GomokuPVP::GetBoxIndex(Point2i mouseCoor)
{
	return Point2i((mouseCoor.x - board.offset) / board.boxParam.sideLength, (mouseCoor.y - board.offset) / board.boxParam.sideLength);
}

bool karthy::GomokuPVP::isPointToReplayIcon(Point2i mouseCoor)
{
	return pow(mouseCoor.x - 1009, 2) + pow(mouseCoor.y - 365, 2) < pow(90, 2);
}

bool karthy::GomokuPVP::isPointToBoard(Point2i mouseCoor)
{
	return (board.offset < mouseCoor.x && mouseCoor.x < board.sideLength + board.offset &&
		board.offset < mouseCoor.y && mouseCoor.y < board.sideLength + board.offset);
}


void karthy::GomokuPVP::handleMoveConsequence(void)
{
	Player theWinner = getWinner();
	if (theWinner == Player::NO_PLAYER)
	{
		if (!board.isFullBox())
		{
			switchPlayer();
		}
		else
		{
			gameStatus = GameStatus::ENDED;
			this->writeText(Text::DRAW);
		}
	}
	else
	{
		gameStatus = GameStatus::ENDED;

		String text;
		Point textOrg;
		Scalar textColor;

		if (theWinner == Player::WHITE_PLAYER)
		{
			this->writeText(Text::WHITE_WIN);
		}
		else
		{
			this->writeText(Text::BLACK_WIN);
		}
	}
}

void karthy::GomokuPVP::writeText(Text text)
{
	if (this->gameFrame.empty()) { return; }

	const int fontFace = FONT_HERSHEY_PLAIN;
	const double fontScale = 6;
	const int thickness = 5;

	String textString;
	Point textOrg;
	Scalar textColor;

	switch (text)
	{
	case karthy::Text::THINKING:
		textString = "THINKING";
		textOrg = Point2i(790, 130);	
		textColor = BLUE_COLOR;
		break;
	case karthy::Text::DRAW:
		textString = "DRAW";
		textOrg = Point2i(880, 130);
		textColor = GRAY_COLOR;
		break;
	case karthy::Text::BLACK_WIN:
		textString = "BLACK WIN";
		textOrg = Point2i(740, 130);
		textColor = BLACK_COLOR;
		break;
	case karthy::Text::WHITE_WIN:
		textString = "WHITE WIN";
		textOrg = Point2i(740, 130);
		textColor = WHITE_COLOR;
		break;
	default:
		break;
	}

	cv::putText(this->gameFrame, textString, textOrg, fontFace, fontScale, textColor, thickness, 8);
	showMap();
}

void karthy::GomokuPVP::eraseText(Text text)
{
	if (this->gameFrame.empty()) { return; }

	const int fontFace = FONT_HERSHEY_PLAIN;
	const double fontScale = 6;
	const int thickness = 5;
	const Scalar textColor = BACKGROUND_COLOR;

	String textString;
	Point textOrg;

	switch (text)
	{
	case karthy::Text::THINKING:
		textString = "THINKING";
		textOrg = Point2i(790, 130);
		break;
	case karthy::Text::DRAW:
		textString = "DRAW";
		textOrg = Point2i(880, 130); 
		break;
	case karthy::Text::BLACK_WIN:
		textString = "BLACK WIN";
		textOrg = Point2i(740, 130);
		break;
	case karthy::Text::WHITE_WIN:
		textString = "WHITE WIN";
		textOrg = Point2i(740, 130);
		break;
	default:
		break;
	}

	cv::putText(this->gameFrame, textString, textOrg, fontFace, fontScale, textColor, thickness, 8);
	showMap();
}

void karthy::GomokuPVP::executeMove(Move move)
{
	latestMove = move;
	board.setBoxStatus(move, (BoxStatus)activePlayer);

	Point2i stoneCoor(move.x * board.boxParam.sideLength + board.offset + board.boxParam.sideLength / 2, move.y * board.boxParam.sideLength + board.offset + board.boxParam.sideLength / 2);
	if (activePlayer == Player::WHITE_PLAYER)
	{
		circle(gameFrame, stoneCoor, stoneRadius, WHITE_COLOR, CV_FILLED);
	}
	else
	{
		circle(gameFrame, stoneCoor, stoneRadius, BLACK_COLOR, CV_FILLED);
	}

	handleMoveConsequence();
	showMap();
}

void karthy::GomokuPVP::run(void)
{
	initGUI();
	newGame();
}