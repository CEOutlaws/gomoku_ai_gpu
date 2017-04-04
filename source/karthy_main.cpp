#include "karthy_gomokuPVE.h"
#include <iostream>

#define BOARD_SIDE_LENGTH (3)
#define STONES_TO_END_GAME (3)

using namespace karthy;

int main(int argc, char** argv)
{	
	GomokuPVE gomokuGame(BOARD_SIDE_LENGTH, STONES_TO_END_GAME, 10);
	gomokuGame.run();

	waitKey(0);
	return 0;
}