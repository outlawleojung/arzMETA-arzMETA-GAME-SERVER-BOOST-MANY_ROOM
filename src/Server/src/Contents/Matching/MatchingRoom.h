#pragma once

#include "../Game/GameRoom.h"

#include <random>

namespace matching
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
		Tile,
		Hint,
		Problem,
		Destroy,
		Award,
		Finish
	};

	class GameData
	{

	public:
		GameData();

		GameState gameState;
		RoundState roundState;

		int roundTotal;
		int roundCount;

		int hintTotal;
		int hintCount;

		int firstRoundInterval = 11000; //게임 시작 후 첫 라운드 시작까지의 시간
		int tileToHintInterval = 500; //타일 정보 전송 후 첫 힌트 제시까지의 시간
		int hintToHintInterval; //힌트와 힌트 사이의 시간
		int quizToDestroyInterval;
		int destroyToFinishInterval; //문제 제시 후 라운드 종료까지의 시간
		int toNextRoundInterval; //라운드 종료 후 다음 라운드 시작까지의 시간
		int showQuizTime;
		int awardingTime = 10000;

		vector<int> hintToHintIntervals;
		vector<int> quizToDestroyIntervals;
		vector<int> destroyToFinishIntervals;
		vector<int> toNextRoundIntervals;
		vector<int> showQuizTimes;

		vector<pair<int, int>> paintConditions;
		vector<vector<vector<bool>>> hintTemplates;

		vector<pair<int, string>> paints;

		vector<string> pictureNames;

		vector<string> players;

		int tileNumber = 16;
		int pictureNumber = 20;

		bool isSoloplay = false;

		std::shared_ptr<std::mt19937> gen;

		bool Init();
		void Clear();
		void SetRound();
		void MakePaints();
		vector<string> GetTiles();
		vector<bool> GetHints();
		string GetProblem();
	};
}

class MatchingRoom : public GameRoom
{
public:
	virtual void Init() override;

	virtual void Handle_C_ENTER(shared_ptr<GameSession>&, Protocol::C_ENTER&) override;

	virtual void Handle_C_MATCHING_GET_HOST(shared_ptr<ClientBase>&, Protocol::C_MATCHING_GET_HOST&) override;
	virtual void Handle_C_MATCHING_START(shared_ptr<ClientBase>&, Protocol::C_MATCHING_START&) override;
	virtual void Handle_C_MATCHING_DIE(shared_ptr<ClientBase>&, Protocol::C_MATCHING_DIE&) override;

	virtual void Leave(shared_ptr<ClientBase> client) override;

	virtual shared_ptr<ClientBase> MakeClient(string clientId, int sessionId) override;

	virtual pair<bool, string> HandleEnter(const Protocol::C_ENTER& pkt) override;
	virtual void SetClientData(shared_ptr<ClientBase> client) override;
	virtual void OnEnterSuccess(shared_ptr<ClientBase> client) override;

	void GetHost(shared_ptr<ClientBase> client);
	void Start(shared_ptr<ClientBase> client);
	void Die(shared_ptr<ClientBase> client);

	void SetHost(string clientId);

	void GameLogic();

	matching::GameData gameData;

	string roomCode;
	string roomName;
	string currentHostId;
};
