#pragma once

#include "../Util/pch.h"
#include "Base/ClientBase.h"
#include "../Session/GameSession.h"

#include <boost/asio.hpp>

class GameSession;

class ClientManager
{
public:
	template<typename T>
	shared_ptr<ClientBase> GetCilent(shared_ptr<GameSession> session, string clientId, string nickName)
	{
		shared_ptr<ClientBase> client = make_shared<T>();

		session->owner = client;
		
		client->session = session;
		client->clientId = clientId;

		{
			boost::unique_lock<boost::recursive_mutex> lock(mtx);

			if (clients.find(clientId) != clients.end())
			{
				session->owner = null;
				client->session = null;
				return null;
			}

			clients.insert({ clientId, client });

			lock.unlock();
		}

		return client;
	}

private:
	map<string, shared_ptr<ClientBase>> clients;

	boost::recursive_mutex mtx;
};