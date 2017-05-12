#include "karthy_GomokuBoard.h"



karthy::BoxStatus karthy::GomokuBoard::getBoxStatus(Index boxIndex)
{
	return (BoxStatus)this->boxStatus[boxIndex.y * this->colCount + boxIndex.x];
}
void karthy::GomokuBoard::setBoxStatus(Index boxIndex, BoxStatus newBoxStatus)
{
	this->boxStatus[boxIndex.y * this->colCount + boxIndex.x] = (uchar)newBoxStatus;
}

void karthy::GomokuBoard::setAllBoxStatus(BoxStatus newBoxStatus)
{
	//this->boxStatus.setTo(Scalar((uchar)newBoxStatus));

	for (int index = 0; index < this->colCount * this->rowCount; index++)
	{
		this->boxStatus[index] = (int)newBoxStatus;
	}
}

void karthy::GomokuBoard::initBoxStatus(int colCount, int rowCount)
{
	//this->boxStatus = Mat(colCount, rowCount, CV_8UC1);
	this->boxStatus = new int[this->colCount * this->rowCount];
	this->setAllBoxStatus(BoxStatus::HAVE_NO_STONE);
}

void karthy::GomokuBoard::initBoxStatus(GomokuBoard& board)
{
	this->initBoxStatus(board.colCount, board.rowCount);
}

bool karthy::GomokuBoard::isFullBox()
{
	for (int index = 0; index < this->colCount * this->rowCount; index++)
	{
		if (this->boxStatus[index] == (int)BoxStatus::HAVE_NO_STONE)
		{
			return false;
		}
	}

	return true;
}

void karthy::GomokuBoard::print(void)
{
#if DEBUG
	for (int boxIndexY = 0; boxIndexY < this->rowCount; boxIndexY++)
	{
		for (int boxIndexX = 0; boxIndexX < this->colCount; boxIndexX++)
		{
			printf("%d", this->boxStatus[boxIndexY * this->colCount + boxIndexX]);
		}
		printf("\n");
	}
#endif
}

void karthy::GomokuBoard::copyTo(int* dstBoxStatus)
{
	for (int index = 0; index < this->colCount * this->rowCount; index++)
	{
		dstBoxStatus[index] = this->boxStatus[index];
	}
}
