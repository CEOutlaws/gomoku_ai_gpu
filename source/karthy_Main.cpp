#include "karthy_GomokuEVE.h"
#include "karthy_GomokuAnalyser.h"
#define BOARD_SIDE_LENGTH (3)
#define STONES_TO_END_GAME (3)

using namespace karthy;

int main(int argc, char** argv)
{	

	/*GomokuPVE gomokuGame(BOARD_SIDE_LENGTH, STONES_TO_END_GAME, 10);
	gomokuGame.run();*/
	
	//
	GomokuAnalyzer* analyzer = new GomokuAnalyzer(true);
	GomokuEVE gomokuGame(BOARD_SIDE_LENGTH, STONES_TO_END_GAME, 1, AgentType::RANDOM, analyzer);
	gomokuGame.run();
	//*/
	return 0;
}