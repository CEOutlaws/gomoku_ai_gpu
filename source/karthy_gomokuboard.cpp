#include "karthy_GomokuBoard.h"

using namespace karthy;

BoxStatus karthy::GomokuBoard::getBoxStatus(Index boxIndex)
{
	return (BoxStatus)this->boxStatus.at<uchar>(boxIndex);
}
void karthy::GomokuBoard::setBoxStatus(Index boxIndex, BoxStatus newBoxStatus)
{
	this->boxStatus.at<uchar>(boxIndex) = (uchar)newBoxStatus;
}

void karthy::GomokuBoard::setAllBoxStatus(BoxStatus newBoxStatus)
{
	this->boxStatus.setTo(Scalar((uchar)newBoxStatus));
}

void karthy::GomokuBoard::initBoxStatus(int colCount, int rowCount)
{
	this->boxStatus = Mat(colCount, rowCount, CV_8UC1);
	this->setAllBoxStatus(BoxStatus::HAVE_NO_STONE);
}

void karthy::GomokuBoard::initBoxStatus(GomokuBoard& board)
{
	this->initBoxStatus(board.colCount, board.rowCount);
}

bool karthy::GomokuBoard::isFullBox()
{
	for (int x = 0; x < colCount; x++)
	{
		for (int y = 0; y < rowCount; y++)
		{
			if (this->getBoxStatus(Index(x, y)) == BoxStatus::HAVE_NO_STONE)
			{
				return false;
			}
		}
	}

	return true;
}