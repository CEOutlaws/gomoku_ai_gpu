#include "karthy_GomokuEVE.h"

karthy::GomokuEVE::GomokuEVE()
{
}

karthy::GomokuEVE::GomokuEVE(uint8_t initBoardCols, uint8_t initStonesToWin, uint8_t aiDepth, AgentType adversaryAgentType, GomokuAnalyzer* initAnalyser) : GomokuPVE(initBoardCols, initStonesToWin, aiDepth)
{
	this->analyserAgent = initAnalyser;
	this->analyzing = initAnalyser->getFlagAnalazing();

	switch (adversaryAgentType)
	{
	case (AgentType::SIMPLE):
		adversaryAgent = new GomokuSimpleAgent(this);
		break;	
	case (AgentType::UNIFORM_RANDOM):
		adversaryAgent = new GomokuUniformRandomAgent(this);
		break;
	case (AgentType::AI):
		adversaryAgent = new GomokuAIAgent(this, RIVAL_MEMORY_PATH);
		break;
	case (AgentType::NORMAL_RANDOM):
		adversaryAgent = new GomokuNormalRandomAgent(this);
		break;
	}	
}


karthy::GomokuEVE::~GomokuEVE()
{
	delete adversaryAgent;
}

void karthy::GomokuEVE::newGame(void)
{
	GomokuPVP::newGame();

	//karthyCEO->getReady(Player::BLACK_PLAYER);
	karthyCEO->getReady(Player::WHITE_PLAYER);

	this->executeMove(this->karthyCEO->takeTurn());

	//adversaryAgent->getReady(Player::WHITE_PLAYER);
	adversaryAgent->getReady(Player::BLACK_PLAYER);

	while (1)
	{
		this->executeMove(adversaryAgent->takeTurn());
		if (this->gameStatus == GameStatus::ENDED)
		{
			if (getWinner() != Player::NO_PLAYER)
			{
				this->analyserAgent->increaseNumWinner(false);
			}
			else
			{
				this->analyserAgent->increaseNumDrawMatch();
			}
			if (this->analyserAgent->isDone()) break;
			replay();
			continue;
		}
		this->executeMove(karthyCEO->takeTurn());
		if (this->gameStatus == GameStatus::ENDED)
		{
			if (getWinner() != Player::NO_PLAYER)
			{
				this->analyserAgent->increaseNumWinner(true);
			}
			else
			{
				this->analyserAgent->increaseNumDrawMatch();
			}
			if (this->analyserAgent->isDone()) break;
			replay();
			continue;
		}
	}
}
void karthy::GomokuEVE::replay(void)
{
	GomokuPVP::newGame();

	if (!this->analyzing)
	{
		this->loadMap();
	}

	Player karthyPreviousPlayer = karthyCEO->getPlayer();
	karthyCEO->getReady(karthyPreviousPlayer);
	//karthyCEO->getReady(!karthyPreviousPlayer);

	if (karthyCEO->getPlayer() == Player::BLACK_PLAYER)
	{
		this->executeMove(this->karthyCEO->takeTurn());
	}

	Player adversaryPreviousPlayer = adversaryAgent->getPlayer();
	adversaryAgent->getReady(adversaryPreviousPlayer);
	//adversaryAgent->getReady(!adversaryPreviousPlayer);
}

void karthy::GomokuEVE::run(void)
{
	if (!this->analyzing) { this->initGUI(); }
	this->newGame();

	if (this->analyzing)
	{
		system("pause");
		return;
	}
	cout << "Done! Press ESC key to confirm";
	while (waitKey(0) != 27);
}

void karthy::GomokuEVE::executeMove(Move move)
{
	if (this->analyzing)
	{
		GomokuGame::executeMove(move);	
	}	
	else
	{
		GomokuPVE::executeMove(move);
	}
}

void karthy::GomokuEVE::MouseHandler(int event, int x, int y)
{
	if (event == EVENT_LBUTTONDOWN ||
		event == EVENT_RBUTTONDOWN ||
		event == EVENT_MBUTTONDOWN)
	{
		//if in Replay Icon
		const Point2i mouseCoor(x, y);
		if (isPointToReplayIcon(mouseCoor))
		{
			replay();
		}
	}
}
