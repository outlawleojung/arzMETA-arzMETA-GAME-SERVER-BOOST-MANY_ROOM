#include "OXRoom.h"

#include "OXClient.h"
#include "../ClientManager.h"
#include "../../ClientPacketHandler.h"
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
		"dev-arowana-mysql.mariadb.database.azure.com", 
		"frontishub@dev-arowana-mysql", 
		"rkskekfk123!@!@"
	);
	con->setSchema("dev_arzmeta_db");

	map<int, bool> answerTypes;
	map<int, int> quizTimeTypes;

	{
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM quizanswertype");

		while (res->next())
		{
			int answerTypeId = res->getInt(1);
			string answerType = res->getString(2);

			if (answerType._Equal("O"))
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

	roomInfoJson["roomId"] = roomId;
	roomInfoJson["roomName"] = roomName;
	roomInfoJson["maxPlayerNumber"] = maxPlayerNumber;
	roomInfoJson["currentPlayerNumber"] = 0;
	roomInfoJson["host"] = "";
	roomInfoJson["isPlaying"] = false;
	roomInfoJson["ip"] = localHostIp;
	roomInfoJson["port"] = tcpPort;
	roomInfo = roomInfoJson.dump();

	//DoTimer(30000, std::function<void()>(
	//	[this]() {
	//		if (this->state != RoomState::Running)
	//			return;

	//		if (this->clients.size() == 0)
	//			Close();
	//	}
	//));
}

void OXRoom::Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) { DoAsync(&OXRoom::Enter, session, pkt); }
void OXRoom::Handle_C_OX_GET_HOST(shared_ptr<ClientBase>& client, Protocol::C_OX_GET_HOST&) { DoAsync(&OXRoom::GetHost, client); }
void OXRoom::Handle_C_OX_START(shared_ptr<ClientBase>& client, Protocol::C_OX_START& pkt) { DoAsync(&OXRoom::Start, client); }
void OXRoom::Handle_C_OX_DIE(shared_ptr<ClientBase>& client, Protocol::C_OX_DIE& pkt) { DoAsync(&OXRoom::Die, client); }

void OXRoom::Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt)
{
	Protocol::S_ENTER res;

	if (state != RoomState::Running)
	{
		session->Disconnect();
		return;
	}

	{
		auto client = clients.find(pkt.clientid());
		if (client != clients.end())
		{
			client->second->DoAsync(&ClientBase::Leave, string("DUPLICATED"));
			DoTimer(1000, &OXRoom::Enter, session, pkt);
			return;
		}
	}

	GLogManager->Log("Session Try to Enter :		", pkt.clientid());

	if (clients.size() >= maxPlayerNumber)
	{
		res.set_result("ROOM_IS_FULL");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		session->Disconnect();
		return;
	}

	if (gameData.gameState != ox::GameState::Idle)
	{
		res.set_result("GAME_IS_PLAYING");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		session->Disconnect();
		return;
	}

	auto client = static_pointer_cast<OXClient>(GClientManager->MakeCilent<OXClient>(session, pkt.clientid(), pkt.nickname(), static_pointer_cast<RoomBase>(shared_from_this())));
	client->enteredTime = std::chrono::system_clock::now();
	clients.insert({ pkt.clientid(), client });

	res.set_result("SUCCESS");
	session->Send(ClientPacketHandler::MakeSendBuffer(res));

	Protocol::S_ADD_CLIENT addClient;
	auto clientInfo = addClient.add_clientinfos();
	clientInfo->set_clientid(pkt.clientid());
	clientInfo->set_nickname(pkt.nickname());
	Broadcast(ClientPacketHandler::MakeSendBuffer(addClient));

	roomInfoJson["currentPlayerNumber"] = roomInfoJson["currentPlayerNumber"].get<int>() + 1;
	roomInfo = roomInfoJson.dump();

	if (currentHostId.empty())
	{
		SetHost(client->clientId);
		//set room visible
		GRoomManager->IndexRoom(static_pointer_cast<RoomBase>(shared_from_this()));
	}
}

void OXRoom::Leave(shared_ptr<ClientBase> client)
{
	if(gameData.gameState == ox::GameState::Playing)
		Die(client);

	GameRoom::Leave(client);

	roomInfoJson["currentPlayerNumber"] = roomInfoJson["currentPlayerNumber"] - 1;
	roomInfo = roomInfoJson.dump();

	if (client->clientId == currentHostId)
	{
		auto nextHost = std::min_element(clients.begin(), clients.end(),
			[](const auto& a, const auto& b) { 	return std::static_pointer_cast<OXClient>(a.second)->enteredTime < std::static_pointer_cast<OXClient>(b.second)->enteredTime; }
		);

		if (nextHost != clients.end())
			SetHost(nextHost->second->clientId);
	}
}

void OXRoom::GetHost(shared_ptr<ClientBase> client)
{
	Protocol::S_OX_HOST host;
	host.set_clientid(currentHostId);
	client->Send(ClientPacketHandler::MakeSendBuffer(host));
}

void OXRoom::Start(shared_ptr<ClientBase> client)
{
	if (gameData.gameState != ox::GameState::Idle)
		return;

	Protocol::S_OX_START start;
	DoAsync(&OXRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(start));

	gameData.gameState = ox::GameState::Playing;
	gameData.roundState = ox::RoundState::Idle;

	roomInfoJson["isPlaying"] = true;
	roomInfo = roomInfoJson.dump();

	DoAsync(&OXRoom::GameLogic);
}

void OXRoom::Die(shared_ptr<ClientBase> client)
{
	for (auto clientId = gameData.players.begin(); clientId != gameData.players.end(); clientId++)
		if (*clientId == client->clientId)
		{
			gameData.players.erase(clientId);
			RemoveObject(client);
			break;
		}
}

void OXRoom::SetHost(string clientId)
{
	auto client = clients.find(clientId);
	if (client == clients.end())
		return;

	if (currentHostId == clientId)
		return;

	currentHostId = clientId;

	roomInfoJson["host"] = client->second->nickname;
	roomInfo = roomInfoJson.dump();

	Protocol::S_OX_HOST hostPkt;
	hostPkt.set_clientid(currentHostId);
	Broadcast(ClientPacketHandler::MakeSendBuffer(hostPkt));
}

void OXRoom::GameLogic()
{
	if (state == RoomState::Closing)
		return;

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
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(roundStart);

		if (gameData.roundCount == 0)
		{
			DoTimer(gameData.firstRoundWaitTime, &OXRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(roundStart));
			DoTimer(gameData.firstRoundWaitTime + gameData.waitingInterval, &OXRoom::GameLogic);
		}
		else
		{
			DoAsync(&OXRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(roundStart));
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

		DoAsync(&OXRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(quiz));

		DoTimer(gameData.playingInterval, &OXRoom::GameLogic);

		gameData.roundState = ox::RoundState::Finish;

		break;
	}
	case ox::RoundState::Finish:
	{
		Protocol::S_OX_ROUND_FINISH roundFinish;
		DoAsync(&OXRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(roundFinish));

		gameData.roundCount++;
		
		if (gameData.players.size() == 0 || (!gameData.isSoloplay && gameData.players.size() == 1) || gameData.roundCount >= gameData.roundTotal)
		{
			Protocol::S_OX_FINISH finish;
			DoAsync(&OXRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(finish));

			gameData.Clear();

			roomInfoJson["isPlaying"] = false;
			roomInfo = roomInfoJson.dump();
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

nlohmann::json OXRoom::ToJson()
{
	nlohmann::json roomInfo;

	roomInfo["roomId"] = roomId;
	roomInfo["roomName"] = roomName;
	roomInfo["maxPlayerNumber"] = maxPlayerNumber;
	roomInfo["currentPlayerNumber"] = clients.size();
	auto host = clients.find(currentHostId);
	if (host != clients.end())
		roomInfo["host"] = host->second->nickname;
	else
		roomInfo["host"] = "";
	roomInfo["isPlaying"] = (gameData.gameState == ox::GameState::Playing);

	roomInfo["ip"] = localHostIp;
	roomInfo["port"] = tcpPort;

	return roomInfo;
}
