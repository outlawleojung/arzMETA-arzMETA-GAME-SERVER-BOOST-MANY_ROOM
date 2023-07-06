#pragma once

#include "../pch.h"
#include "Base/ClientBase.h"
#include "Base/RoomBase.h"
#include "../Session/GameSession.h"

#include <boost/asio.hpp>

#include <soci/soci.h>

class GameSession;

class ClientManager
{
public:
	template<typename T>
	shared_ptr<ClientBase> MakeCilent(string clientId, int sessionId, shared_ptr<RoomBase> enteredRoom)
	{
		lock_guard<std::recursive_mutex> lock(mtx);

		auto _sessionId = sessionIds.find(clientId);
		if (_sessionId == sessionIds.end() || _sessionId->second != sessionId)
		{
			GLogManager->Log("Make Client Fail! : ", clientId, " ", to_string(sessionId));
			return nullptr;
		}

		shared_ptr<ClientBase> client = make_shared<T>();

		client->clientId = clientId;
		client->sessionId = sessionId;
		client->enteredRoom = enteredRoom;
		client->enteredRoomId = enteredRoom->roomId;

		GLogManager->Log("Make Client Success : ", clientId, " to ", client->enteredRoomId);

		auto prevClient = clients.find(clientId);
		if (prevClient != clients.end())
			prevClient->second->DoTimer(0, &ClientBase::Leave, string("SERVER_CHANGE"));

		clients[clientId] = client;

		soci::session sql(*DBConnectionPool);

		sql << "INSERT INTO memberconnectinfo (membercode, roomId) VALUES (:id, :room) ON DUPLICATE KEY UPDATE roomId = VALUES(roomId)",
			soci::use(clientId), soci::use(enteredRoom->roomId);

		sql.close();

		return client;
	}

	void RemoveClient(shared_ptr<ClientBase> client)
	{
		lock_guard<std::recursive_mutex> lock(mtx);

		auto _client = clients.find(client->clientId);
		if (_client->second.get() == client.get())
		{
			soci::session sql(*DBConnectionPool);

			sql << "DELETE FROM memberconnectinfo WHERE membercode = :id", soci::use(client->clientId);

			sql.close();

			clients.erase(_client);
		}
	}

	shared_ptr<ClientBase> GetClient(string clientId)
	{
		lock_guard<std::recursive_mutex> lock(mtx);

		auto client = clients.find(clientId);
		if (client == clients.end())
			return nullptr;

		return client->second;
	}

	int SetSessionId(string clientId)
	{
		lock_guard<std::recursive_mutex> lock(mtx);
		
		auto sessionId = sessionIds.find(clientId);
		if (sessionId == sessionIds.end())
		{
			GLogManager->Log("From Login, Add to SessionIds : ", clientId);
			sessionIds.insert({ clientId, 0 });
			return 0;
		}
		else
		{
			auto client = clients.find(clientId);
			if (client != clients.end())
			{
				GLogManager->Log("From Login, Duplicated Client : ", clientId);
				client->second->DoTimer(0, &ClientBase::Leave, string("DUPLICATED"));
			}

			sessionId->second = sessionId->second + 1;
			GLogManager->Log("From Login, Session SessionId : ", clientId, " ", to_string(sessionId->second));
			return sessionId->second;
		}
	}

private:
	map<string, shared_ptr<ClientBase>> clients;
	map<string, int> sessionIds;

	std::recursive_mutex mtx;
};