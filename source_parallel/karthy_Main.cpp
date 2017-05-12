#include "Game\karthy_GomokuEVE.h"
#include "Analyser\karthy_GomokuAnalyser.h"

int main(int argc, char** argv)
{
	karthy::GomokuAnalyzer* analyzer = new karthy::GomokuAnalyzer(true, 5);
	karthy::GomokuEVE gomokuGame(BOARD_SIDE_LENGTH, STONES_TO_END_GAME, 1, karthy::AgentType::AI, analyzer);
	gomokuGame.run();

	return 0;
}

/*
std::cout << "GpuAction: " << sizeof(karthy::cuda::GpuAction) << endl;
std::cout << "GpuMatrix: " << sizeof(karthy::cuda::GpuMatrix) << endl;
std::cout << "GpuActionArray: " << sizeof(karthy::cuda::GpuActionArray) << endl;
std::cout << "kernel consist 1 action 2 matrix 1 actionarray: " << sizeof(karthy::cuda::GpuActionArray) + 2 * sizeof(karthy::cuda::GpuMatrix) + sizeof(karthy::cuda::GpuAction) << endl;

system("pause");
return 0;
//*/

/*GomokuPVE gomokuGame(BOARD_SIDE_LENGTH, STONES_TO_END_GAME, 10);
gomokuGame.run();*/