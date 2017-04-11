#include "karthy_gomokuPVP.h"

using namespace karthy;

BoxStatus karthy::GomokuPVP::GomokuBoard::getBoxStatus(Index boxIndex)
{
	return (BoxStatus)boxStatus.at<uchar>(boxIndex);
}
void karthy::GomokuPVP::GomokuBoard::setBoxStatus(Index boxIndex, BoxStatus newBoxStatus)
{
	boxStatus.at<uchar>(boxIndex) = (uchar)newBoxStatus;
}

void karthy::GomokuPVP::GomokuBoard::setAllBoxStatus(BoxStatus newBoxStatus)
{
	boxStatus.setTo(Scalar((uchar)newBoxStatus));
}

void karthy::GomokuPVP::GomokuBoard::initBoxStatus(int colCount, int rowCount)
{
	boxStatus = Mat(colCount, rowCount, CV_8UC1);
}

bool karthy::GomokuPVP::GomokuBoard::isFullBox()
{
	for (int x = 0; x < colCount; x++)
	{
		for (int y = 0; y < rowCount; y++)
		{
			if (getBoxStatus(Index(x, y)) == BoxStatus::HAVE_NO_STONE)
			{
				return false;
			}
		}
	}

	return true;
}