#pragma once

#include "../pch.h"
#include "Base/ClientBase.h"
#include "Base/RoomBase.h"
#include "../Session/GameSession.h"

#include <boost/asio.hpp>

#include <mutex>

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
			return nullptr;

		{
			auto client = clients.find(clientId);
			if (client != clients.end())
			{
				client->second->DoAsync(&ClientBase::Leave, string("SERVER_CHANGE"));
				clients.erase(client);
			}
		}

		shared_ptr<ClientBase> client = make_shared<T>();

		client->clientId = clientId;
		client->sessionId = sessionId;
		client->enteredRoom = enteredRoom;

		clients.insert({ clientId, client });

		return client;
	}

	void RemoveClient(shared_ptr<ClientBase> client)
	{
		lock_guard<std::recursive_mutex> lock(mtx);

		auto _client = clients.find(client->clientId);
		if (_client != clients.end() && _client->second.get() == client.get())
		{
			clients.erase(_client);
			//sessionIds.erase(client->clientId);
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

		cout << "test1" << endl;

		auto sessionId = sessionIds.find(clientId);

		cout << "test2" << endl;

		if (sessionId == sessionIds.end())
		{
			cout << "test3" << endl;

			sessionIds.insert({ clientId, 0 });

			cout << "test4" << endl;

			return 0;
		}
		else
		{
			cout << "test4" << endl;

			auto client = clients.find(clientId);
			if (client != clients.end())
				client->second->DoAsync(&ClientBase::Leave, string("DUPLICATED"));

			cout << "test5" << endl;

			sessionId->second = sessionId->second + 1;
			return sessionId->second;
		}
	}

private:
	map<string, shared_ptr<ClientBase>> clients;
	map<string, int> sessionIds;

	std::recursive_mutex mtx;
};
