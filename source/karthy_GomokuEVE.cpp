#include "karthy_GomokuEVE.h"

using namespace karthy;

karthy::GomokuEVE::GomokuEVE()
{
}

karthy::GomokuEVE::GomokuEVE(uint8_t initBoardCols, uint8_t initStonesToWin, uint8_t aiDepth, AgentType adversaryAgentType, GomokuAnalyzer* initAnalyser) : GomokuPVE(initBoardCols, initStonesToWin, aiDepth)
{
	//adversaryAgent = new GomokuSimpleAgent(this);
	
	this->analyserAgent = initAnalyser;
	this->analyzing = initAnalyser->getFlagAnalazing();
	switch (adversaryAgentType)
	{
	case (AgentType::SIMPLE):
		adversaryAgent = new GomokuSimpleAgent(this);
		break;	
	case (AgentType::RANDOM):
		adversaryAgent = new GomokuRandomAgent(this);
		break;
	case (AgentType::AI):
		adversaryAgent = new GomokuAIAgent(this);
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

	karthyCEO->getReady(Player::BLACK_PLAYER);
	this->executeMove(this->karthyCEO->takeTurn());
	
	adversaryAgent->getReady(Player::WHITE_PLAYER);

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
	karthyCEO->getReady(!karthyPreviousPlayer);

	if (karthyCEO->getPlayer() == Player::BLACK_PLAYER)
	{
		this->executeMove(this->karthyCEO->takeTurn());
	}

	Player adversaryPreviousPlayer = adversaryAgent->getPlayer();
	adversaryAgent->getReady(!adversaryPreviousPlayer);
}

void karthy::GomokuEVE::run(void)
{
	if (!this->analyzing) { this->initGUI(); }
	this->newGame();
}

void karthy::GomokuEVE::executeMove(Move move)
{
	if (this->analyzing)
	{
		this->latestMove = move;
		this->board.setBoxStatus(move, (BoxStatus)this->activePlayer);
		Player theWinner = getWinner();
		if (theWinner == Player::NO_PLAYER)
		{
			if (!board.isFullBox())
			{
				switchPlayer();
			}
			else
			{
				gameStatus = GameStatus::ENDED;
			}
		}
		else
		{
			gameStatus = GameStatus::ENDED;
		}
	}	
	else
	{
		GomokuPVE::executeMove(move);
		//waitKey(WAIT_MOVE_TIME);
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
