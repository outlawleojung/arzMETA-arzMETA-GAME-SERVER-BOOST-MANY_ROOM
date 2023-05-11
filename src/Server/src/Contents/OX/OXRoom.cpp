#include "OXRoom.h"

#include "OXClient.h"
#include "../ClientManager.h"
#include "../../PacketManager.h"
#include "../../Session/GameSession.h"

#include "../RoomManager.h"

#include <functional>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

ox::GameData::GameData() 
	: gameState(GameState::Idle)
	, roundState(RoundState::Idle)
	, roundCount(0)
	, isSoloplay(false)
{
	std::random_device rd;
}

bool ox::GameData::Init()
{
	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::ResultSet* res;

	driver = get_driver_instance();

	con = driver->connect(
		DBDomain, 
		DBUsername, 
		DBPassword
	);
	con->setSchema(DBSchema);

	map<int, bool> answerTypes;
	map<int, int> quizTimeTypes;

	{
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM quizanswertype");

		while (res->next())
		{
			int answerTypeId = res->getInt(1);
			string answerType = res->getString(2);

			if (answerType == "O")
				answerTypes.insert({ answerTypeId, true });
			else
				answerTypes.insert({ answerTypeId, false });
		}

	}

	{
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM quizquestionanswer");

		while (res->next())
		{
			int quizId = res->getInt(2);
			int answerTypeId = res->getInt(3);

			auto answerType = answerTypes.find(answerTypeId);
			if (answerType == answerTypes.end())
				return false;

			quiz.push_back(
				std::pair<int, bool>(quizId, answerType->second)
			);
		}
	}

	{
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM quizlevel");

		roundTotal = res->rowsCount();

		while (res->next())
		{
			waitingIntervals.push_back(res->getInt(2));
			playingIntervals.push_back(res->getInt(3));
		}
	}

	{
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM quiztimetype");

		while (res->next())
		{
			int quizTimeTypeId = res->getInt(1);
			int quizTime = res->getInt(2);

			quizTimeTypes.insert({ quizTimeTypeId , quizTime });
		}
	}

	{
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM quizroundtime");

		while (res->next())
		{
			int quizTimeTypeId = res->getInt(2);

			auto quizTimeType = quizTimeTypes.find(quizTimeTypeId);
			if (quizTimeType == quizTimeTypes.end())
				return false;

			roundTimes.push_back(quizTimeType->second);
		}
	}

	delete res;
	delete stmt;
	delete con;

	return true;
}

void ox::GameData::Clear()
{
	gameState = ox::GameState::Idle;
	roundState = ox::RoundState::Idle;
	roundCount = 0;
	isSoloplay = false;
	players.clear();
	quizIndex.clear();
	mistModeRound = 0;
	pieceModeRound = 0;
	pieceModePanel.clear();
}

void ox::GameData::ShuffleQuiz()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, quiz.size() - 1);

	quizIndex.clear();
	for (int i = 0; i < quiz.size(); i++)
		quizIndex.push_back(i);

	int first;
	int second;
	int temp;

	for (int i = 0; i < quiz.size() * 2; i++)
	{
		first = distrib(gen);
		second = distrib(gen);
		if (first == second)
			continue;

		temp = quizIndex[first];
		quizIndex[first] = quizIndex[second];
		quizIndex[second] = temp;
	}
}

void ox::GameData::SetModeRound()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	{
		std::uniform_int_distribution<> distrib(1, 4);
		mistModeRound = distrib(gen);
	}

	{
		std::uniform_int_distribution<> distrib(5, 8);
		pieceModeRound = distrib(gen);
	}

	{
		std::uniform_int_distribution<> distrib(0, 11);

		for (int i = 0; i < 5; i++)
			pieceModePanel.push_back(0);

		for (int i = 0; i < 5; i++)
			pieceModePanel.push_back(1);

		for (int i = 0; i < 5; i++)
			pieceModePanel.push_back(2);

		int first;
		int second;
		int temp;

		for (int i = 0; i < pieceModePanel.size() * 2; i++)
		{
			first = distrib(gen);
			second = distrib(gen);
			if (first == second)
				continue;

			temp = pieceModePanel[first];
			pieceModePanel[first] = pieceModePanel[second];
			pieceModePanel[second] = temp;
		}
	}
}

void ox::GameData::SetRound()
{
	waitingInterval = waitingIntervals.at(roundCount);
	playingInterval = playingIntervals.at(roundCount);
	roundTime = roundTimes.at(roundCount);

	currentQuiz = quiz[quizIndex[roundCount]];
}

void OXRoom::Init()
{
	state = RoomState::Running;

	if(!gameData.Init())
		Close();

	roomCode = roomId;

	roomInfo["roomId"] = roomId;
	roomInfo["roomName"] = roomName;
	roomInfo["maxPlayerNumber"] = maxPlayerNumber;
	roomInfo["currentPlayerNumber"] = 0;
	roomInfo["host"] = "";
	roomInfo["isPlaying"] = false;
	roomInfo["sceneName"] = sceneName;

	roomInfo["roomType"] = roomTypeToString(type);

	DoTimer(30000, std::function<void()>(
		[this]() {
			if (this->state != RoomState::Running)
				return;

			if (this->clients.size() == 0)
				Close();
		}
	));
}

void OXRoom::Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) { DoAsync(&OXRoom::Enter, session, pkt); }
void OXRoom::Handle_C_OX_GET_HOST(shared_ptr<ClientBase>& client, Protocol::C_OX_GET_HOST&) { DoAsync(&OXRoom::GetHost, client); }
void OXRoom::Handle_C_OX_START(shared_ptr<ClientBase>& client, Protocol::C_OX_START& pkt) { DoAsync(&OXRoom::Start, client); }
void OXRoom::Handle_C_OX_DIE(shared_ptr<ClientBase>& client, Protocol::C_OX_DIE& pkt) { DoAsync(&OXRoom::Die, client); }

void OXRoom::Leave(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	if(gameData.gameState == ox::GameState::Playing)
		Die(client);

	GameRoom::Leave(client);

	roomInfo["currentPlayerNumber"] = roomInfo["currentPlayerNumber"].get<int>() - 1;

	if (client->clientId == currentHostId)
	{
		auto nextHost = std::min_element(clients.begin(), clients.end(),
			[](const auto& a, const auto& b) { 	return std::static_pointer_cast<OXClient>(a.second)->enteredTime < std::static_pointer_cast<OXClient>(b.second)->enteredTime; }
		);

		if (nextHost != clients.end())
			SetHost(nextHost->second->clientId);
	}
}

shared_ptr<ClientBase> OXRoom::MakeClient(string clientId, int sessionId)
{
	return GClientManager->MakeCilent<OXClient>(clientId, sessionId, static_pointer_cast<RoomBase>(shared_from_this()));
}

pair<bool, string> OXRoom::HandleEnter(const Protocol::C_ENTER& pkt)
{
	if (clients.size() >= maxPlayerNumber)
		return { false, "ROOM_IS_FULL" };

	if (gameData.gameState != ox::GameState::Idle)
		return { false, "GAME_IS_PLAYING" };

	return { true, "SUCCESS" };
}

void OXRoom::SetClientData(shared_ptr<ClientBase> client)
{
	static_pointer_cast<OXClient>(client)->enteredTime = std::chrono::system_clock::now();
}

void OXRoom::OnEnterSuccess(shared_ptr<ClientBase> client)
{
	GameRoom::OnEnterSuccess(client);

	roomInfo["currentPlayerNumber"] = roomInfo["currentPlayerNumber"].get<int>() + 1;

	if (currentHostId.empty())
	{
		SetHost(client->clientId);
		GRoomManager->IndexRoom(static_pointer_cast<RoomBase>(shared_from_this()));
	}
}

void OXRoom::GetHost(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	Protocol::S_OX_HOST host;
	host.set_clientid(currentHostId);
	client->Send(PacketManager::MakeSendBuffer(host));
}

void OXRoom::Start(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	if (gameData.gameState != ox::GameState::Idle)
		return;

	Protocol::S_OX_START start;
	DoAsync(&OXRoom::Broadcast, PacketManager::MakeSendBuffer(start));

	gameData.gameState = ox::GameState::Playing;
	gameData.roundState = ox::RoundState::Idle;

	roomInfo["isPlaying"] = true;

	DoAsync(&OXRoom::GameLogic);
}

void OXRoom::Die(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	for (auto clientId = gameData.players.begin(); clientId != gameData.players.end(); clientId++)
		if (*clientId == client->clientId)
		{
			gameData.players.erase(clientId);
			RemoveObject(static_pointer_cast<GameClient>(client));
			break;
		}
}

void OXRoom::SetHost(string clientId)
{
	if (state != RoomState::Running) return;

	auto client = clients.find(clientId);
	if (client == clients.end())
		return;

	if (currentHostId == clientId)
		return;

	currentHostId = clientId;

	roomInfo["host"] = client->second->nickname;

	Protocol::S_OX_HOST hostPkt;
	hostPkt.set_clientid(currentHostId);
	Broadcast(PacketManager::MakeSendBuffer(hostPkt));
}

void OXRoom::GameLogic()
{
	if (state != RoomState::Running) return;

	switch (gameData.roundState)
	{
	case ox::RoundState::Idle:
	{
		for (auto client = clients.begin(); client != clients.end(); client++)
			gameData.players.push_back(client->second->clientId);

		if (clients.size() == 1)
			gameData.isSoloplay = true;

		gameData.ShuffleQuiz();
		gameData.SetModeRound();

		gameData.roundState = ox::RoundState::Start;
		DoAsync(&OXRoom::GameLogic);
		break;
	}
	case ox::RoundState::Start:
	{
		gameData.SetRound();

		Protocol::S_OX_ROUND_START roundStart;
		if (gameData.roundCount == gameData.mistModeRound)
			roundStart.set_mode("GHOST");
		else if (gameData.roundCount == gameData.pieceModeRound)
			roundStart.set_mode("FRAG");
		else
			roundStart.set_mode("NORMAL");
		auto sendBuffer = PacketManager::MakeSendBuffer(roundStart);

		if (gameData.roundCount == 0)
		{
			DoTimer(gameData.firstRoundWaitTime, &OXRoom::Broadcast, PacketManager::MakeSendBuffer(roundStart));
			DoTimer(gameData.firstRoundWaitTime + gameData.waitingInterval, &OXRoom::GameLogic);
		}
		else
		{
			DoAsync(&OXRoom::Broadcast, PacketManager::MakeSendBuffer(roundStart));
			if (gameData.roundCount == gameData.mistModeRound || gameData.roundCount == gameData.pieceModeRound || gameData.roundCount == gameData.mistModeRound + 1 || gameData.roundCount == gameData.pieceModeRound + 1)
				DoTimer(gameData.waitingInterval + gameData.modeWaitTime, &OXRoom::GameLogic);
			else
				DoTimer(gameData.waitingInterval, &OXRoom::GameLogic);
		}

		gameData.roundState = ox::RoundState::Playing;

		break;
	}
	case ox::RoundState::Playing:
	{
		Protocol::S_OX_QUIZ quiz;
		quiz.set_quiz(gameData.currentQuiz.first);
		quiz.set_answer(gameData.currentQuiz.second);
		quiz.set_timetodestory(gameData.roundTime);

		if (gameData.roundCount == gameData.pieceModeRound)
			for (auto panel : gameData.pieceModePanel)
				quiz.add_panel(panel);

		DoAsync(&OXRoom::Broadcast, PacketManager::MakeSendBuffer(quiz));

		DoTimer(gameData.playingInterval, &OXRoom::GameLogic);

		gameData.roundState = ox::RoundState::Finish;

		break;
	}
	case ox::RoundState::Finish:
	{
		Protocol::S_OX_ROUND_FINISH roundFinish;
		DoAsync(&OXRoom::Broadcast, PacketManager::MakeSendBuffer(roundFinish));

		gameData.roundCount++;
		
		if (gameData.players.size() == 0 || (!gameData.isSoloplay && gameData.players.size() == 1) || gameData.roundCount >= gameData.roundTotal)
		{
			Protocol::S_OX_FINISH finish;
			DoAsync(&OXRoom::Broadcast, PacketManager::MakeSendBuffer(finish));

			gameData.Clear();

			roomInfo["isPlaying"] = false;
		}
		else
		{
			DoAsync(&OXRoom::GameLogic);

			gameData.roundState = ox::RoundState::Start;
		}
		break;
	}
	}
}