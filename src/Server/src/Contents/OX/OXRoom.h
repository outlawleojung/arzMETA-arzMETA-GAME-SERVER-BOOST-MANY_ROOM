#pragma once

#include "../Game/GameRoom.h"

#include <random>

namespace ox
{
	enum GameState
	{
		Idle,
		Playing
	};

	enum class RoundState
	{
		Idle,
		Start,
		Playing,
		Finish
	};

	class GameData
	{

	public:
		GameData();

		GameState gameState;
		RoundState roundState;

		std::vector<std::pair<int, bool>> quiz;
		std::vector<int> quizIndex;
		std::pair<int, bool> currentQuiz;

		int roundTotal;
		int roundCount;

		int mistModeRound;
		int pieceModeRound;
		vector<int> pieceModePanel;

		vector<int> waitingIntervals;
		vector<int> playingIntervals;
		vector<int> roundTimes;

		int waitingInterval;
		int playingInterval;
		int roundTime;
		int modeWaitTime = 2000;
		int firstRoundWaitTime = 6000;

		vector<string> players;

		bool isSoloplay = false;

		bool Init();
		void Clear();

		void ShuffleQuiz();
		void SetModeRound();
		void SetRound();
	};
}

class OXRoom : public GameRoom
{
public:
	virtual void Init() override;

	virtual void Handle_C_ENTER(shared_ptr<GameSession>&, Protocol::C_ENTER&) override;

	virtual void Handle_C_OX_GET_HOST(shared_ptr<ClientBase>&, Protocol::C_OX_GET_HOST&) override;
	virtual void Handle_C_OX_START(shared_ptr<ClientBase>&, Protocol::C_OX_START&) override;
	virtual void Handle_C_OX_DIE(shared_ptr<ClientBase>&, Protocol::C_OX_DIE&) override;

	virtual void Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt) override;
	virtual void Leave(shared_ptr<ClientBase> client) override;

	void GetHost(shared_ptr<ClientBase> client);
	void Start(shared_ptr<ClientBase> client);
	void Die(shared_ptr<ClientBase> client);

	void SetHost(string clientId);

	void GameLogic();

	ox::GameData gameData;

	string roomCode;
	string roomName;
	string currentHostId;
};
