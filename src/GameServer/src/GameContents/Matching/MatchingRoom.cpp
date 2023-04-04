#include "MatchingRoom.h"

#include "MatchingClient.h"
#include "../ClientManager.h"
#include "../../ClientPacketHandler.h"
#include "../../Session/GameSession.h"

#include <functional>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

matching::GameData::GameData() 
	: gameState(GameState::Idle)
	, roundCount(0)
	, roundState(RoundState::Idle)
	, isSoloplay(false)
{
	std::random_device rd;
	gen = std::make_shared<std::mt19937>(rd());
}

bool matching::GameData::Init()
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

	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT * FROM jumpingmatchinglevel");

	roundTotal = res->rowsCount();

	hintToHintIntervals.clear();
	quizToDestroyIntervals.clear();
	destroyToFinishIntervals.clear();
	toNextRoundIntervals.clear();
	showQuizTimes.clear();

	paintConditions.clear();
	hintTemplates.clear();

	while (res->next())
	{
		hintToHintIntervals.push_back(std::stoi(res->getString(3)));
		quizToDestroyIntervals.push_back(std::stoi(res->getString(4)));
		destroyToFinishIntervals.push_back(std::stoi(res->getString(5)));
		toNextRoundIntervals.push_back(std::stoi(res->getString(6)));
		showQuizTimes.push_back(std::stoi(res->getString(7)));

		int paintNumber = std::stoi(res->getString(9));

		paintConditions.push_back({ paintNumber, std::stoi(res->getString(10)) });

		string hintTemplateString = string(res->getString(11));

		vector<vector<bool>> hintTemplatesForRound;

		for (int i = 0; i < hintTemplateString.size(); i++)
		{
			std::vector<bool> hintTemplate;

			for (int j = 0; j < paintNumber; j++)
			{
				hintTemplate.push_back(hintTemplateString[i] - '0');
				i++;
			}

			hintTemplatesForRound.push_back(hintTemplate);
		}

		hintTemplates.push_back(hintTemplatesForRound);
	}

	delete res;
	delete stmt;
	delete con;

	for (int i = 1; i <= pictureNumber; i++)
		pictureNames.push_back(std::to_string(i));

	return true;
}

void matching::GameData::Clear()
{
	gameState = matching::GameState::Idle;

	roundCount = 0;
	roundState = matching::RoundState::Idle;

	players.clear();

	isSoloplay = false;
}

void matching::GameData::SetRound()
{
	hintToHintInterval = hintToHintIntervals[roundCount];
	quizToDestroyInterval = quizToDestroyIntervals[roundCount];
	destroyToFinishInterval = destroyToFinishIntervals[roundCount];
	toNextRoundInterval = toNextRoundIntervals[roundCount];
	showQuizTime = showQuizTimes[roundCount];

	MakePaints();

	hintTotal = hintTemplates[roundCount].size();
	hintCount = 0;
}

void matching::GameData::MakePaints()
{
	std::vector<string> pictures(pictureNames);
	std::vector<int> randomTiles;

	{
		std::uniform_int_distribution<> distrib(0, pictureNumber - 1);
		std::string tempPicName;

		for (int i = 0; i < 100; i++)
		{
			int first = distrib(*gen);
			int second = distrib(*gen);
			if (first == second)
				continue;
			tempPicName = pictures[first];
			pictures[first] = pictures[second];
			pictures[second] = tempPicName;
		}
	}

	{
		std::uniform_int_distribution<> distrib(0, tileNumber - 1);
		int tempRandomTile;

		for (int i = 0; i < tileNumber; i++)
			randomTiles.push_back(i);

		for (int i = 0; i < 100; i++)
		{
			int first = distrib(*gen);
			int second = distrib(*gen);
			if (first == second)
				continue;
			tempRandomTile = randomTiles[first];
			randomTiles[first] = randomTiles[second];
			randomTiles[second] = tempRandomTile;
		}
	}

	paints.clear();

	auto paintNumber = paintConditions[roundCount].first;
	auto pictureNumber = paintConditions[roundCount].second;

	for (int i = 0; i < paintNumber; i++)
		paints.push_back(std::pair<int, string>(randomTiles[i], pictures[i % pictureNumber]));
}

vector<string> matching::GameData::GetTiles()
{
	std::vector<string> tiles(tileNumber);

	for (int i = 0; i < tileNumber; i++)
		tiles[i] = "X";

	for (auto it = paints.begin(); it != paints.end(); it++)
		tiles[it->first] = it->second;

	return tiles;
}

vector<bool> matching::GameData::GetHints()
{
	std::vector<bool> _hints(tileNumber);
	for (int i = 0; i < tileNumber; i++)
		_hints[i] = false;

	auto hintTemplate = hintTemplates[roundCount][hintCount];

	for (int i = 0; i < hintTemplate.size(); i++)
		if (hintTemplate[i])
			_hints[paints[i].first] = true;

	return _hints;
}

string matching::GameData::GetProblem()
{
	std::uniform_int_distribution<> distrib;

	distrib = std::uniform_int_distribution<>(0, paintConditions[roundCount].second - 1);

	return paints[distrib(*gen)].second;
}

void MatchingRoom::Init()
{
	state = RoomState::Running;

	if(!gameData.Init())
		Close();

	//this->DoTimer(30000, std::function<void()>(
	//	[this]() {
	//		if (this->state != RoomState::Running)
	//			return;

	//		if (this->clients.size() == 0)
	//			Close();
	//	}
	//));
}

void MatchingRoom::Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) { DoAsync(&MatchingRoom::Enter, session, pkt); }
void MatchingRoom::Handle_C_MATCHING_START(shared_ptr<ClientBase>& client, Protocol::C_MATCHING_START& pkt) { DoAsync(&MatchingRoom::Start, client); }
void MatchingRoom::Handle_C_MATCHING_DIE(shared_ptr<ClientBase>& client, Protocol::C_MATCHING_DIE& pkt) { DoAsync(&MatchingRoom::Die, client); }

void MatchingRoom::Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt)
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
			DoTimer(1000, &MatchingRoom::Enter, session, pkt);
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

	if (gameData.gameState != matching::GameState::Idle)
	{
		res.set_result("GAME_IS_PLAYING");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		session->Disconnect();
		return;
	}

	auto client = static_pointer_cast<MatchingClient>(GClientManager->MakeCilent<MatchingClient>(session, pkt.clientid(), pkt.nickname(), static_pointer_cast<RoomBase>(shared_from_this())));

	if (clients.size() == 0)
	{
		currentHostId = client->clientId;
		//set room visible
		DoAsync(&MatchingRoom::SetHost, client->clientId);
	}

	clients.insert({ pkt.clientid(), client });

	res.set_result("SUCCESS");
	session->Send(ClientPacketHandler::MakeSendBuffer(res));
}

void MatchingRoom::Leave(shared_ptr<ClientBase> client)
{
	if (client->clientId == currentHostId)
	{
		auto nextHost = std::min_element(clients.begin(), clients.end(), 
			[](const auto& a, const auto& b) { 	return std::static_pointer_cast<MatchingClient>(a.second)->enteredTime < std::static_pointer_cast<MatchingClient>(b.second)->enteredTime; }
		);

		if (nextHost != clients.end())
			SetHost(nextHost->second->clientId);
	}

	if(gameData.gameState == matching::GameState::Playing)
		Die(client);

	GameRoom::Leave(client);
}

void MatchingRoom::Start(shared_ptr<ClientBase> client)
{
	if (gameData.gameState != matching::GameState::Idle)
		return;

	Protocol::S_MATCHING_START start;
	DoAsync(&MatchingRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(start));

	gameData.gameState = matching::GameState::Playing;
	gameData.roundState = matching::RoundState::Idle;

	this->DoAsync(&MatchingRoom::GameLogic);
}

void MatchingRoom::Die(shared_ptr<ClientBase> client)
{
	for (auto clientId = gameData.players.begin(); clientId != gameData.players.end(); clientId++)
		if (*clientId == client->clientId)
		{
			gameData.players.erase(clientId);
			RemoveObject(client);
			break;
		}
}

void MatchingRoom::SetHost(string clientId)
{
	auto client = clients.find(clientId);
	if (client == clients.end())
		return;

	if (currentHostId == clientId)
		return;

	if (!currentHostId.empty())
	{
		auto prevHost = clients.find(currentHostId);
		if (prevHost != clients.end())
		{
			//do somthing
		}
	}

	currentHostId = clientId;

	Protocol::S_MATCHING_HOST hostPkt;
	client->second->Send(ClientPacketHandler::MakeSendBuffer(hostPkt));
}

void MatchingRoom::GameLogic()
{
	if (state == RoomState::Closing)
		return;

	switch (gameData.roundState)
	{
	case matching::RoundState::Idle:
	{
		for (auto client = clients.begin(); client != clients.end(); client++)
			gameData.players.push_back(client->second->clientId);

		if (clients.size() == 1)
			gameData.isSoloplay = true;

		gameData.roundState = matching::RoundState::Start;
		DoAsync(&MatchingRoom::GameLogic);
		break;
	}
	case matching::RoundState::Start:
	{
		gameData.SetRound();

		Protocol::S_MATCHING_ROUND_START roundStart;

		if (gameData.roundCount == 0)
		{
			this->DoTimer(gameData.firstRoundInterval, &MatchingRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(roundStart));
			this->DoTimer(gameData.firstRoundInterval, &MatchingRoom::GameLogic);
		}
		else
		{
			DoAsync(&MatchingRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(roundStart));
			DoAsync(&MatchingRoom::GameLogic);
		}

		gameData.roundState = matching::RoundState::Tile;

		break;
	}
	case matching::RoundState::Tile:
	{
		auto tiles = gameData.GetTiles();

		Protocol::S_MATCHING_TILES tilesPkt;
		for (int i = 0; i < tiles.size(); i++)
			tilesPkt.add_tiles(tiles[i]);

		DoAsync(&MatchingRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(tilesPkt));

		this->DoTimer(gameData.tileToHintInterval, &MatchingRoom::GameLogic);

		gameData.roundState = matching::RoundState::Hint;

		break;
	}
	case matching::RoundState::Hint:
	{
		if (gameData.hintCount < gameData.hintTotal)
		{
			auto hints = gameData.GetHints();

			Protocol::S_MATCHING_HINT hintsPkt;
			for (int i = 0; i < gameData.tileNumber; i++)
				hintsPkt.add_hints(hints[i]);

			DoAsync(&MatchingRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(hintsPkt));

			this->DoTimer(gameData.hintToHintInterval, &MatchingRoom::GameLogic);

			gameData.hintCount++;
		}
		else
		{
			DoAsync(&MatchingRoom::GameLogic);

			gameData.roundState = matching::RoundState::Problem;
		}
		break;
	}
	case matching::RoundState::Problem:
	{
		auto problem = gameData.GetProblem();

		Protocol::S_MATCHING_PROBLEM problemPkt;
		problemPkt.set_problem(problem);
		problemPkt.set_timetodestroy(gameData.quizToDestroyInterval);
		DoAsync(&MatchingRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(problemPkt));

		Protocol::S_MATCHING_QUIZ_DISAPPEAR disappear;
		this->DoTimer(gameData.showQuizTime, &MatchingRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(disappear));

		this->DoTimer(gameData.quizToDestroyInterval, &MatchingRoom::GameLogic);

		gameData.roundState = matching::RoundState::Destroy;

		break;
	}
	case matching::RoundState::Destroy:
	{
		Protocol::S_MATCHING_DESTROY destroy;
		DoAsync(&MatchingRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(destroy));

		this->DoTimer(gameData.destroyToFinishInterval, &MatchingRoom::GameLogic);

		gameData.roundState = matching::RoundState::Finish;

		break;
	}
	case matching::RoundState::Finish:
	{
		Protocol::S_MATCHING_ROUND_FINISH roundFinish;
		DoAsync(&MatchingRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(roundFinish));

		gameData.roundCount++;
		
		if (gameData.players.size() == 0 || (!gameData.isSoloplay && gameData.players.size() == 1) || gameData.roundCount >= gameData.roundTotal)
		{
			Protocol::S_MATCHING_FINISH finish;
			DoAsync(&MatchingRoom::Broadcast, ClientPacketHandler::MakeSendBuffer(finish));

			gameData.Clear();
		}
		else
		{
			this->DoTimer(gameData.toNextRoundInterval, &MatchingRoom::GameLogic);

			gameData.roundState = matching::RoundState::Start;
		}
		break;
	}
	}
}

nlohmann::json MatchingRoom::ToJson()
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
	roomInfo["isPlaying"] = (gameData.gameState == matching::GameState::Playing);

	roomInfo["ip"] = "192.168.0.47";
	roomInfo["port"] = 7777;

	return roomInfo;
}
