#include "MatchingRoom.h"

#include "MatchingClient.h"
#include "../ClientManager.h"
#include "../../PacketManager.h"
#include "../../Session/GameSession.h"

#include "../RoomManager.h"

#include <functional>

matching::GameData::GameData() 
	: gameState(GameState::Idle)
	, roundCount(0)
	, roundState(RoundState::Idle)
	, isSoloplay(false)
{
	std::random_device rd;
	gen = std::make_shared<std::mt19937>(rd());
}

#include <soci/soci.h>

bool matching::GameData::Init()
{
	soci::session sql(*DBConnectionPool);

	if (!sql.is_connected())
	{
		GLogManager->Log("Mysql Connection Disconnected. Reconnect");
		sql.reconnect();
	}

	soci::rowset<soci::row> rs = (sql.prepare << "SELECT * FROM jumpingmatchinglevel");

	hintToHintIntervals.clear();
	quizToDestroyIntervals.clear();
	destroyToFinishIntervals.clear();
	toNextRoundIntervals.clear();
	showQuizTimes.clear();

	paintConditions.clear();
	hintTemplates.clear();

	for (auto it = rs.begin(); it != rs.end(); ++it)
	{
		const soci::row& row = *it;

		hintToHintIntervals.push_back(row.get<int>(2));
		quizToDestroyIntervals.push_back(row.get<int>(3));
		destroyToFinishIntervals.push_back(row.get<int>(4));
		toNextRoundIntervals.push_back(row.get<int>(5));
		showQuizTimes.push_back(row.get<int>(6));

		int paintNumber = row.get<int>(8);

		paintConditions.push_back({ paintNumber, row.get<int>(9) });

		string hintTemplateString = row.get<string>(10);

		std::vector<std::vector<bool>> hintTemplatesForRound;

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

	roundTotal = hintTemplates.size();

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

	roomCode = roomId;

	roomInfo["roomId"] = roomId;
	roomInfo["roomName"] = roomName;
	roomInfo["maxPlayerNumber"] = maxPlayerNumber;
	roomInfo["currentPlayerNumber"] = 0;
	roomInfo["host"] = "";
	roomInfo["isPlaying"] = false;
	roomInfo["sceneName"] = sceneName;

	roomInfo["roomType"] = roomTypeToString(type);

	this->DoTimer(30000, std::function<void()>(
		[this]() {
			if (this->state != RoomState::Running)
				return;

			if (this->clients.size() == 0)
				Close();
		}
	));
}

void MatchingRoom::Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) { DoAsync(&MatchingRoom::Enter, session, pkt); }
void MatchingRoom::Handle_C_MATCHING_GET_HOST(shared_ptr<ClientBase>& client, Protocol::C_MATCHING_GET_HOST&) { DoAsync(&MatchingRoom::GetHost, client); }
void MatchingRoom::Handle_C_MATCHING_START(shared_ptr<ClientBase>& client, Protocol::C_MATCHING_START& pkt) { DoAsync(&MatchingRoom::Start, client); }
void MatchingRoom::Handle_C_MATCHING_DIE(shared_ptr<ClientBase>& client, Protocol::C_MATCHING_DIE& pkt) { DoAsync(&MatchingRoom::Die, client); }

void MatchingRoom::Leave(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	if(gameData.gameState == matching::GameState::Playing)
		Die(client);

	GameRoom::Leave(client);

	roomInfo["currentPlayerNumber"] = roomInfo["currentPlayerNumber"].get<int>() - 1;

	if (client->clientId == currentHostId)
	{
		auto nextHost = std::min_element(clients.begin(), clients.end(),
			[](const auto& a, const auto& b) { 	return std::static_pointer_cast<MatchingClient>(a.second)->enteredTime < std::static_pointer_cast<MatchingClient>(b.second)->enteredTime; }
		);

		if (nextHost != clients.end())
			SetHost(nextHost->second->clientId);
	}
}

shared_ptr<ClientBase> MatchingRoom::MakeClient(string clientId, int sessionId)
{
	return GClientManager->MakeCilent<MatchingClient>(clientId, sessionId, static_pointer_cast<RoomBase>(shared_from_this()));
}

pair<bool, string> MatchingRoom::HandleEnter(const Protocol::C_ENTER& pkt)
{
	if (clients.size() >= maxPlayerNumber)
		return { false, "ROOM_IS_FULL" };

	if (gameData.gameState != matching::GameState::Idle)
		return { false, "GAME_IS_PLAYING" };

	return { true, "SUCCESS" };
}

void MatchingRoom::SetClientData(shared_ptr<ClientBase> client)
{
	static_pointer_cast<MatchingClient>(client)->enteredTime = std::chrono::system_clock::now();
}

void MatchingRoom::OnEnterSuccess(shared_ptr<ClientBase> client)
{
	GameRoom::OnEnterSuccess(client);

	roomInfo["currentPlayerNumber"] = roomInfo["currentPlayerNumber"].get<int>() + 1;

	if (currentHostId.empty())
	{
		SetHost(client->clientId);
		GRoomManager->IndexRoom(static_pointer_cast<RoomBase>(shared_from_this()));
	}
}

void MatchingRoom::GetHost(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	Protocol::S_MATCHING_HOST host;
	host.set_clientid(currentHostId);
	client->Send(PacketManager::MakeSendBuffer(host));
}

void MatchingRoom::Start(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	if (gameData.gameState != matching::GameState::Idle) return;

	Protocol::S_MATCHING_START start;
	DoAsync(&MatchingRoom::Broadcast, PacketManager::MakeSendBuffer(start));

	gameData.gameState = matching::GameState::Playing;
	gameData.roundState = matching::RoundState::Idle;

	roomInfo["isPlaying"] = true;

	this->DoAsync(&MatchingRoom::GameLogic);
}

void MatchingRoom::Die(shared_ptr<ClientBase> client)
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

void MatchingRoom::SetHost(string clientId)
{
	if (state != RoomState::Running) return;

	auto client = clients.find(clientId);
	if (client == clients.end())
		return;

	if (currentHostId == clientId)
		return;

	currentHostId = clientId;

	roomInfo["host"] = client->second->nickname;

	Protocol::S_MATCHING_HOST hostPkt;
	hostPkt.set_clientid(currentHostId);
	Broadcast(PacketManager::MakeSendBuffer(hostPkt));
}

void MatchingRoom::GameLogic()
{
	if (state != RoomState::Running) return;

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
			this->DoTimer(gameData.firstRoundInterval, &MatchingRoom::Broadcast, PacketManager::MakeSendBuffer(roundStart));
			this->DoTimer(gameData.firstRoundInterval, &MatchingRoom::GameLogic);
		}
		else
		{
			DoAsync(&MatchingRoom::Broadcast, PacketManager::MakeSendBuffer(roundStart));
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

		DoAsync(&MatchingRoom::Broadcast, PacketManager::MakeSendBuffer(tilesPkt));

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

			DoAsync(&MatchingRoom::Broadcast, PacketManager::MakeSendBuffer(hintsPkt));

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
		DoAsync(&MatchingRoom::Broadcast, PacketManager::MakeSendBuffer(problemPkt));

		Protocol::S_MATCHING_QUIZ_DISAPPEAR disappear;
		this->DoTimer(gameData.showQuizTime, &MatchingRoom::Broadcast, PacketManager::MakeSendBuffer(disappear));

		this->DoTimer(gameData.quizToDestroyInterval, &MatchingRoom::GameLogic);

		gameData.roundState = matching::RoundState::Destroy;

		break;
	}
	case matching::RoundState::Destroy:
	{
		Protocol::S_MATCHING_DESTROY destroy;
		DoAsync(&MatchingRoom::Broadcast, PacketManager::MakeSendBuffer(destroy));

		this->DoTimer(gameData.destroyToFinishInterval, &MatchingRoom::GameLogic);

		gameData.roundState = matching::RoundState::Finish;

		break;
	}
	case matching::RoundState::Finish:
	{
		Protocol::S_MATCHING_ROUND_FINISH roundFinish;
		DoAsync(&MatchingRoom::Broadcast, PacketManager::MakeSendBuffer(roundFinish));

		gameData.roundCount++;

		if (gameData.players.size() == 0)
		{
			Protocol::S_MATCHING_FINISH finish;
			DoAsync(&MatchingRoom::Broadcast, PacketManager::MakeSendBuffer(finish));

			roomInfo["isPlaying"] = false;

			gameData.Clear();
		}
		else if ((!gameData.isSoloplay && gameData.players.size() == 1) || gameData.roundCount >= gameData.roundTotal)
		{
			Protocol::S_MATCHING_AWARD award;
			for (int i = 0; i < gameData.players.size(); i++)
				award.add_winners(gameData.players[i]);

			DoAsync(&MatchingRoom::Broadcast, PacketManager::MakeSendBuffer(award));

			gameData.roundState = matching::RoundState::Award;
			DoTimer(gameData.awardingTime, &MatchingRoom::GameLogic);
		}
		else
		{
			this->DoTimer(gameData.toNextRoundInterval, &MatchingRoom::GameLogic);

			gameData.roundState = matching::RoundState::Start;
		}
		break;
	}
	case matching::RoundState::Award:
	{
		ClearObject();

		Protocol::S_MATCHING_FINISH finish;
		DoAsync(&MatchingRoom::Broadcast, PacketManager::MakeSendBuffer(finish));

		roomInfo["isPlaying"] = false;

		gameData.Clear();
	}
	}
}
