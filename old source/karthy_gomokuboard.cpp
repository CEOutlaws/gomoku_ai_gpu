#include "karthy_gomokuPVP.h"

using namespace karthy;

boxstatus_c karthy::gomokuPVP_c::gomokuboard_c::GetBoxStatus(index boxIndex)
{
	return boxStatus[boxIndex.x][boxIndex.y];
}
void karthy::gomokuPVP_c::gomokuboard_c::SetBoxStatus(index boxIndex, boxstatus_c newBoxStatus)
{
	boxStatus[boxIndex.x][boxIndex.y] = newBoxStatus;
}

void karthy::gomokuPVP_c::gomokuboard_c::SetAllBoxStatus(boxstatus_c newBoxStatus)
{
	if (boxStatus == NULL)
	{
		boxStatus = new boxstatus_c *[colCount];
		for (int i = 0; i < rowCount; i++)
		{
			boxStatus[i] = new boxstatus_c[rowCount];
		}
	}

	for (int x = 0; x < colCount; x++)
	{
		for (int y = 0; y < rowCount; y++)
		{
			boxStatus[x][y] = newBoxStatus;
		}
	}
}