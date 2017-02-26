#include "karthy_gomokuPVP.h"

using namespace karthy;

karthy::gomokuPVP_c::gomokuPVP_c()
{
	board.cols = 19;
	board.sideLength = board.cols * board.box.sideLength;
	_stonesToWin = 5;
}

karthy::gomokuPVP_c::gomokuPVP_c(int boardCols, char initStonesToWin)
{
	board.cols = boardCols;
	board.sideLength = board.cols * board.box.sideLength;
	_stonesToWin = initStonesToWin;
}

karthy::gomokuPVP_c::~gomokuPVP_c()
{
	
}

void karthy::gomokuPVP_c::MouseHandler(int event, int x, int y)
{
	const Point2i mouseCoor(x, y);
	if (event == EVENT_LBUTTONDOWN ||
		event == EVENT_RBUTTONDOWN ||
		event == EVENT_MBUTTONDOWN)
	{
		//if in Replay Icon
		if (IsPointToReplayIcon(mouseCoor))
		{
			Replay();
		}
		else if (gameStatus == gamestatus_c::PLAYING && IsPointToBoard(mouseCoor))
		{
			index pointedBoxIndex = GetBoxIndex(mouseCoor);

			if (GetBoxStatus(pointedBoxIndex) == boxstatus_c::HAVE_NO_STONE)
			{
				ExecuteMove(pointedBoxIndex);
			}


		}
	}
}

void karthy::gomokuPVP_c::MouseHandler(int event, int x, int y, int flags, void* userdata)
{
	gomokuPVP_c* pGomokuPVP_c = reinterpret_cast<gomokuPVP_c*>(userdata);
	pGomokuPVP_c->MouseHandler(event, x, y);
}

void karthy::gomokuPVP_c::NewGame(void)
{
	activePlayer = (bool)player_c::BLACK_PLAYER;
	SetAllBoxStatus(boxstatus_c::HAVE_NO_STONE);
	latestMove = Point2i(-1, -1);
	gameStatus = gamestatus_c::PLAYING;
}
void karthy::gomokuPVP_c::LoadMap()
{
	gameFrame = imread("caro_background.jpg", 1);
	for (int pixelX = board.sideLength; pixelX < board.maxSideLength; pixelX++)
	{
		for (int pixelY = 0; pixelY < board.maxSideLength + board.box.marginThickness; pixelY++)
		{
			gameFrame.at<Vec3b>(Point(pixelX + board.offset + board.box.marginThickness, pixelY + board.offset)) = BACKGROUND_COLOR;
		}
	}
	for (int pixelX = 0; pixelX < board.sideLength + board.box.marginThickness; pixelX++)
	{
		for (int pixelY = board.sideLength; pixelY < board.maxSideLength; pixelY++)
		{
			gameFrame.at<Vec3b>(Point(pixelX + board.offset, pixelY + board.offset + board.box.marginThickness)) = BACKGROUND_COLOR;
		}
	}
	imshow("My Window", gameFrame);
}

void karthy::gomokuPVP_c::Replay(void)
{
	LoadMap();
	NewGame();
}

void karthy::gomokuPVP_c::GUIInit(void)
{
	LoadMap();
	//set the callback function for any mouse event
	setMouseCallback("My Window", MouseHandler, this);
}

Point2i karthy::gomokuPVP_c::GetBoxIndex(Point2i mouseCoor)
{
	return Point2i((mouseCoor.x - board.offset) / board.box.sideLength, (mouseCoor.y - board.offset) / board.box.sideLength);
}

bool karthy::gomokuPVP_c::IsPointToReplayIcon(Point2i mouseCoor)
{
	return pow(mouseCoor.x - 1009, 2) + pow(mouseCoor.y - 365, 2) < pow(90, 2);
}

bool karthy::gomokuPVP_c::IsPointToBoard(Point2i mouseCoor)
{
	return (board.offset < mouseCoor.x && mouseCoor.x < board.sideLength + board.offset &&
			board.offset < mouseCoor.y && mouseCoor.y < board.sideLength + board.offset);
}


boxstatus_c karthy::gomokuPVP_c::GetBoxStatus(index boxIndex)
{
	return board.box.status[boxIndex.x][boxIndex.y];
}
void karthy::gomokuPVP_c::SetBoxStatus(index boxIndex, boxstatus_c newBoxStatus)
{
	board.box.status[boxIndex.x][boxIndex.y] = newBoxStatus;
}

void karthy::gomokuPVP_c::SwitchPlayer(void)
{
	activePlayer = !(activePlayer);
}

player_c karthy::gomokuPVP_c::GetWinner(void)
{
	player_c winner = player_c::NO_PLAYER;

	if (IsVerticalEnd() || IsHorizontalEnd() || IsDiagonalRLEnd() || IsDiagonalLREnd())
	{
		winner = (player_c)(GetBoxStatus(gomokuPVP_c::latestMove));
	}

	return winner;
}

void karthy::gomokuPVP_c::HandleMoveConsequence(void)
{
	if (GetWinner() == player_c::NO_PLAYER)
	{
		SwitchPlayer();
	}
	else
	{
		gameStatus = gamestatus_c::ENDED;

		String text;
		Point textOrg;
		Scalar textColor;

		if (GetWinner() == player_c::WHITE_PLAYER)
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

void karthy::gomokuPVP_c::ExecuteMove(index boxIndex)
{
	Point2i stoneCoor(boxIndex.x * board.box.sideLength + board.offset + board.box.sideLength / 2, boxIndex.y * board.box.sideLength + board.offset + board.box.sideLength / 2);
	if (activePlayer == (bool)(player_c::WHITE_PLAYER))
	{
		SetBoxStatus(boxIndex, boxstatus_c::HAVE_WHITE_STONE);
		circle(gameFrame, stoneCoor, stoneRadius, WHITE_COLOR, CV_FILLED);
	}
	else
	{
		SetBoxStatus(boxIndex, boxstatus_c::HAVE_BLACK_STONE);
		circle(gameFrame, stoneCoor, stoneRadius, BLACK_COLOR, CV_FILLED);
	}

	latestMove = boxIndex;
	HandleMoveConsequence();
	imshow("My Window", gameFrame);
}

void karthy::gomokuPVP_c::SetAllBoxStatus(boxstatus_c newBoxStatus)
{
	for (int x = 0; x < board.cols; x++)
	{
		for (int y = 0; y < board.cols; y++)
		{
			board.box.status[x][y] = newBoxStatus;
		}
	}
}

bool karthy::gomokuPVP_c::IsVerticalEnd(void)
{
	char stoneCount = 1;

	for (int boxIndexY = latestMove.y + 1; boxIndexY < board.cols; boxIndexY++)
	{
		if (GetBoxStatus(latestMove) != GetBoxStatus(index(latestMove.x, boxIndexY)))
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
		if (GetBoxStatus(latestMove) != GetBoxStatus(index(latestMove.x, boxIndexY)))
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

bool karthy::gomokuPVP_c::IsHorizontalEnd(void)
{
	char stoneCount = 1;

	for (int boxIndexX = latestMove.x + 1; boxIndexX < board.cols; boxIndexX++)
	{
		boxstatus_c a = GetBoxStatus(index(boxIndexX, latestMove.y));
		if (GetBoxStatus(latestMove) != GetBoxStatus(index(boxIndexX, latestMove.y)))
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
		boxstatus_c a = GetBoxStatus(index(boxIndexX, latestMove.y));
		boxstatus_c b = GetBoxStatus(latestMove);
		if (GetBoxStatus(latestMove) != GetBoxStatus(index(boxIndexX, latestMove.y)))
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

bool karthy::gomokuPVP_c::IsDiagonalLREnd(void)
{
	char stoneCount = 1;

	for (
		int boxIndexX = latestMove.x + 1, boxIndexY = latestMove.y + 1;
		boxIndexX < board.cols && boxIndexY < board.cols;
		boxIndexX++, boxIndexY++
		)
	{
		if (GetBoxStatus(latestMove) != GetBoxStatus(index(boxIndexX, boxIndexY)))
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
		if (GetBoxStatus(latestMove) != GetBoxStatus(index(boxIndexX, boxIndexY)))
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

bool karthy::gomokuPVP_c::IsDiagonalRLEnd(void)
{
	char stoneCount = 1;

	for (
		int boxIndexX = latestMove.x + 1, boxIndexY = latestMove.y - 1;
		boxIndexX < board.cols && boxIndexY >= 0;
		boxIndexX++, boxIndexY--
		)
	{
		if (GetBoxStatus(latestMove) != GetBoxStatus(index(boxIndexX, boxIndexY)))
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
		boxIndexX >= 0 && boxIndexY < board.cols;
		boxIndexX--, boxIndexY++
		)
	{
		if (GetBoxStatus(latestMove) != GetBoxStatus(index(boxIndexX, boxIndexY)))
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
void karthy::gomokuPVP_c::Run(void)
{
	GUIInit();
	NewGame();
}