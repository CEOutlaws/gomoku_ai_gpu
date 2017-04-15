#include "karthy_GomokuEVE.h"

#define BOARD_SIDE_LENGTH (7)
#define STONES_TO_END_GAME (5)

using namespace karthy;

int main(int argc, char** argv)
{	///*

	GomokuPVE gomokuGame(BOARD_SIDE_LENGTH, STONES_TO_END_GAME, 10);
	gomokuGame.run();
	//*/
	/*
	GomokuEVE gomokuGame(BOARD_SIDE_LENGTH, STONES_TO_END_GAME, 10);
	gomokuGame.run();
	//*/
	return 0;
}