#include "karthy_GomokuAnalyser.h"

karthy::GomokuAnalyzer::GomokuAnalyzer(bool flag, uint16_t maxNumAnalyze)
{
	this->database.open(DATABASE_ANALYZER_PATH);

	this->flagAnalazing = flag;

	this->numWinAIAgent = 0;
	this->numWinAgent2 = 0;
	this->numMatch = 0;
	this->numDrawMatch = 0;
	this->numAnalyze = 0;
	this->maxNumAnalyze = maxNumAnalyze;

	this->timeStart = clock();
}

karthy::GomokuAnalyzer::~GomokuAnalyzer()
{
	this->database.close();
}
bool karthy::GomokuAnalyzer::getFlagAnalazing()
{
	return this->flagAnalazing;
}
void karthy::GomokuAnalyzer::increaseNumWinner(bool isAIAgent)
{
	this->numMatch++;
	if (isAIAgent == true)
	{
		this->numWinAIAgent++;
	}
	else {
		this->numWinAgent2++;
	}
	this->statisticWin();
	//printf("so tran: %d. AI win : %d . Agent win : %d .Draw: %d \n ", this->numMatch, this->numWinAIAgent, this->numWinAgent2, this->numDrawMatch);
}
void karthy::GomokuAnalyzer::increaseNumDrawMatch()
{
	this->numMatch++;
	this->numDrawMatch++;
	this->statisticWin();
	
	//printf("so tran: %d. AI win : %d . Agent win : %d .Draw: %d \n ", this->numMatch, this->numWinAIAgent, this->numWinAgent2, this->numDrawMatch);
}
void karthy::GomokuAnalyzer::statisticWin()
{
	if (this->numMatch == 100)
	{
		printf("thang/tong = %d/%d\n", this->numWinAIAgent, this->numMatch);
		printf("hoa/tong = %d/%d\n", this->numDrawMatch, this->numMatch);
		printf("thua/tong = %d/%d\n", this->numWinAgent2, this->numMatch);
		database << this->numWinAIAgent <<","
				<< this->numWinAgent2 <<","
				<< this->numDrawMatch << std::endl;

		this->numWinAIAgent = 0;
		this->numWinAgent2 = 0;
		this->numMatch = 0;
		this->numDrawMatch = 0;
		this->numAnalyze++;

		clock_t timeFinish = clock();
		printf("It took %d seconds to execute the 100 match.\n", (timeFinish - this->timeStart) / CLOCKS_PER_SEC);
		this->timeStart = clock();
	}
}

bool karthy::GomokuAnalyzer::isDone(void)
{
	return this->numAnalyze >= this->maxNumAnalyze;
}
