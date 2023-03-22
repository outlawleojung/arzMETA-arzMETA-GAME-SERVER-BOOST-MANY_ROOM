#pragma once

#include "../Util/pch.h"
#include "Base/ClientBase.h"
#include "Base/RoomBase.h"
#include "../Session/GameSession.h"

#include <boost/asio.hpp>

class GameSession;

class ClientManager
{
public:
	template<typename T>
	shared_ptr<ClientBase> MakeCilent(shared_ptr<GameSession> session, string clientId, string nickname, shared_ptr<RoomBase> room)
	{
		boost::lock_guard<boost::recursive_mutex> lock(mtx);

		//session id �� �ֽ����� Ȯ��
		//�ֽ��� �ƴ϶�� return nullptr

		{
			auto client = clients.find(clientId);
			if (client != clients.end())
			{
				//client �� session id �� �ڽŰ� �������� �ʴٸ� Duplicated
				//client �� session id �� �ڽŰ� �����ϴٸ� ����� ����?
				client->second->DoAsync(&ClientBase::Leave, string("LEAVED"));
				clients.erase(client);
			}
		}

		shared_ptr<ClientBase> client = make_shared<T>();

		client->session = session;
		client->clientId = clientId;
		client->nickname = nickname;
		client->enteredRoom = room;

		session->owner = client;

		clients.insert({ clientId, client });

		return client;
	}

	void RemoveClient(shared_ptr<ClientBase> _client)
	{
		boost::lock_guard<boost::recursive_mutex> lock(mtx);

		auto client = clients.find(_client->clientId);
		if (client == clients.end())
			return;

		if (_client->enteredRoom->roomId != client->second->enteredRoom->roomId)
			return;

		clients.erase(client);
	}

	shared_ptr<ClientBase> GetClient(string clientId)
	{
		boost::lock_guard<boost::recursive_mutex> lock(mtx);

		auto client = clients.find(clientId);
		if (client == clients.end())
			return nullptr;

		return client->second;
	}

private:
	map<string, shared_ptr<ClientBase>> clients;

	boost::recursive_mutex mtx;
};