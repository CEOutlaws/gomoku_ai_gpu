#pragma once
#include "karthy_Common.h"
namespace karthy
{
	class GomokuBoard
	{
	public:
		//////There are boxes on the board/////

		//box's parameter
		struct
		{
			const int sideLength = 37;
			const int marginThickness = 2;	//margin cover around the box
		} boxParam;

		///box's status
		//each box have its status: WHITE/BLACK/NO STONE IS PLACED on the box
		Mat boxStatus;

		BoxStatus getBoxStatus(Index boxIndex);
		void setBoxStatus(Index boxIndex, BoxStatus newBoxStatus);
		void setAllBoxStatus(BoxStatus newBoxStatus);
		void initBoxStatus(int colCount, int rowCount);
		void initBoxStatus(GomokuBoard& board);
		bool isFullBox(void);

		///////////board's parameter////////////

		//number of collumn
		int colCount;
		//number of rows, because cols = rows so I use refference
		int &rowCount = colCount;
		//length
		int sideLength;
		const int offset = 7;
		//game support upto 19x19 board
		const int maxCols = 19;
		const int maxSideLength = maxCols * boxParam.sideLength;
		///////////////////////////////////////
	};
}