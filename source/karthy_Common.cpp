#include "karthy_Common.h"

using namespace karthy;

Player karthy::operator!(Player &operand)
{
	Player result = operand;
	
	if (operand == Player::BLACK_PLAYER)
	{
		result = Player::WHITE_PLAYER;
	}
	else if (operand == Player::WHITE_PLAYER)
	{
		result = Player::BLACK_PLAYER;
	}

	return result;
}

void karthy::displayBoard(void)
{
}

void karthy::printBoard(Mat board)
{
	for (int i = 0; i < board.cols; i++)
	{
		for (int j = 0; j < board.rows; j++)
		{
			printf("%d", board.at<uchar>(Point2i(i, j)));
		}
		printf("\n");
	}
}

BoxStatus karthy::operator!(BoxStatus &operand)
{
	BoxStatus result = operand;

	if (operand == BoxStatus::HAVE_BLACK_STONE)
	{
		result = BoxStatus::HAVE_WHITE_STONE;
	}
	else if (operand == BoxStatus::HAVE_WHITE_STONE)
	{
		result = BoxStatus::HAVE_BLACK_STONE;
	}

	return result;
}
