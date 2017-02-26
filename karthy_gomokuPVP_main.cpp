
#include "karthy_gomokuPVP.h"
#include <iostream>

#define BOARD_SIDE_LENGTH (3)
#define STONES_TO_END_GAME (3)
using namespace karthy;

int main(int argc, char** argv)
{
	
	gomokuPVP_c gomokuGame(BOARD_SIDE_LENGTH, STONES_TO_END_GAME);
	gomokuGame.Run();

	waitKey(0);
	return 0;

}