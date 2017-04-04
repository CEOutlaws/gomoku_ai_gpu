#include "karthy_gomokuPVP.h"

using namespace karthy;

karthy::GomokuPVP::GomokuPVP() :GomokuPVP(19, 5) {}

karthy::GomokuPVP::GomokuPVP(int boardCols, char initStonesToWin)
{
	board.colCount = boardCols;
	board.sideLength = board.colCount * board.boxParam.sideLength;
	_stonesToWin = initStonesToWin;
	
	board.initBoxStatus(board.colCount, board.rowCount);
	board.setAllBoxStatus(BoxStatus::HAVE_NO_STONE);
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
			Replay();
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

void karthy::GomokuPVP::newGame(void)
{
	activePlayer = Player::BLACK_PLAYER;
	latestMove = NULL_MOVE;
	gameStatus = GameStatus::PLAYING;
}

void karthy::GomokuPVP::loadMap()
{
	gameFrame = imread("caro_background.jpg", 1);
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
	imshow("My Window", gameFrame);
	waitKey(1);
}
void karthy::GomokuPVP::Replay(void)
{
	board.setAllBoxStatus(BoxStatus::HAVE_NO_STONE);
	loadMap();
	newGame();
}

void karthy::GomokuPVP::initGUI(void)
{
	loadMap();
	//set the callback function for any mouse event
	setMouseCallback("My Window", MouseHandler, this);
}

Index karthy::GomokuPVP::GetBoxIndex(Point2i mouseCoor)
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

void karthy::GomokuPVP::switchPlayer(void)
{
	activePlayer = !activePlayer;
}

Player karthy::GomokuPVP::getWinner(void)
{
	Player winner = Player::NO_PLAYER;
	if (latestMove == NULL_MOVE) { return winner; }

	if (isVerticalEnd() || isHorizontalEnd() || isDiagonalRLEnd() || isDiagonalLREnd())
	{
		winner = (Player)(board.getBoxStatus(GomokuPVP::latestMove));
	}

	return winner;
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

			String text = "DRAW";
			Point textOrg = Point2i(880, 130);
			Scalar textColor = GRAY_COLOR;
			int fontFace = FONT_HERSHEY_PLAIN;
			double fontScale = 6;
			int thickness = 5;
			cv::putText(gameFrame, text, textOrg, fontFace, fontScale, textColor, thickness, 8);
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
			text = "WHITE WIN";
			textOrg = Point2i(750, 130);
			textColor = WHITE_COLOR;
		}
		else
		{
			text = "BLACK WIN";
			textOrg = Point2i(740, 130);
			textColor = BLACK_COLOR;
		}

		int fontFace = FONT_HERSHEY_PLAIN;
		double fontScale = 6;
		int thickness = 5;

		cv::putText(gameFrame, text, textOrg, fontFace, fontScale, textColor, thickness, 8);
	}
}

void karthy::GomokuPVP::executeMove(Index boxIndex)
{
	Point2i stoneCoor(boxIndex.x * board.boxParam.sideLength + board.offset + board.boxParam.sideLength / 2, boxIndex.y * board.boxParam.sideLength + board.offset + board.boxParam.sideLength / 2);
	if (activePlayer == Player::WHITE_PLAYER)
	{
		board.setBoxStatus(boxIndex, BoxStatus::HAVE_WHITE_STONE);
		circle(gameFrame, stoneCoor, stoneRadius, WHITE_COLOR, CV_FILLED);
	}
	else
	{
		board.setBoxStatus(boxIndex, BoxStatus::HAVE_BLACK_STONE);
		circle(gameFrame, stoneCoor, stoneRadius, BLACK_COLOR, CV_FILLED);
	}

	latestMove = boxIndex;
	handleMoveConsequence();
	showMap();
}

bool karthy::GomokuPVP::isVerticalEnd(void)
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

bool karthy::GomokuPVP::isHorizontalEnd(void)
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

bool karthy::GomokuPVP::isDiagonalLREnd(void)
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

bool karthy::GomokuPVP::isDiagonalRLEnd(void)
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
void karthy::GomokuPVP::run(void)
{
	initGUI();
	newGame();
}