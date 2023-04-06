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

		int firstRoundInterval = 11000; //���� ���� �� ù ���� ���۱����� �ð�
		int tileToHintInterval = 500; //Ÿ�� ���� ���� �� ù ��Ʈ ���ñ����� �ð�
		int hintToHintInterval; //��Ʈ�� ��Ʈ ������ �ð�
		int quizToDestroyInterval;
		int destroyToFinishInterval; //���� ���� �� ���� ��������� �ð�
		int toNextRoundInterval; //���� ���� �� ���� ���� ���۱����� �ð�
		int showQuizTime;

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
	MatchingRoom(vector<string> scenes) : GameRoom(scenes)
	{}

	virtual void Init() override;

	virtual void Handle_C_ENTER(shared_ptr<GameSession>&, Protocol::C_ENTER&) override;

	virtual void Handle_C_MATCHING_GET_HOST(shared_ptr<ClientBase>&, Protocol::C_MATCHING_GET_HOST&) override;
	virtual void Handle_C_MATCHING_START(shared_ptr<ClientBase>&, Protocol::C_MATCHING_START&) override;
	virtual void Handle_C_MATCHING_DIE(shared_ptr<ClientBase>&, Protocol::C_MATCHING_DIE&) override;

	virtual void Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt) override;
	virtual void Leave(shared_ptr<ClientBase> client) override;

	void GetHost(shared_ptr<ClientBase> client);
	void Start(shared_ptr<ClientBase> client);
	void Die(shared_ptr<ClientBase> client);

	void SetHost(string clientId);

	void GameLogic();

	virtual nlohmann::json ToJson() override;

	matching::GameData gameData;

	string roomCode;
	string roomName;
	string currentHostId;
};
