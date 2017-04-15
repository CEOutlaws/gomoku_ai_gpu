#include "karthy_GomokuEVE.h"

#define BOARD_SIDE_LENGTH (11)
#define STONES_TO_END_GAME (5)

using namespace karthy;

int main(int argc, char** argv)
{	/*

	GomokuPVE gomokuGame(BOARD_SIDE_LENGTH, STONES_TO_END_GAME, 10);
	gomokuGame.run();
	//*/
	///*
	GomokuEVE gomokuGame(BOARD_SIDE_LENGTH, STONES_TO_END_GAME, 1, AgentType::RANDOM, false);
	gomokuGame.run();
	//*/
	return 0;
}