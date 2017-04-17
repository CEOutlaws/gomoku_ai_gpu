#include "karthy_GomokuEVE.h"
#include "karthy_GomokuAnalyser.h"

#define BOARD_SIDE_LENGTH (11)
#define STONES_TO_END_GAME (5)

using namespace karthy;

int main(int argc, char** argv)
{

	/*GomokuPVE gomokuGame(BOARD_SIDE_LENGTH, STONES_TO_END_GAME, 10);
	gomokuGame.run();*/

	//
	GomokuAnalyzer* analyzer = new GomokuAnalyzer(false);
	GomokuEVE gomokuGame(BOARD_SIDE_LENGTH, STONES_TO_END_GAME, 1, AgentType::UNIFORM_RANDOM, analyzer);
	gomokuGame.run();
	//*/
	return 0;
}