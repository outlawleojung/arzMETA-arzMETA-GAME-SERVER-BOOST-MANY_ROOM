#include "RoomBase.h"
#include "../RoomManager.h"
#include "ClientBase.h"
#include "../../PacketManager.h"
#include "../ClientManager.h"

RoomBase::RoomBase()
	: state(RoomState::Idle)
{
	GLogManager->Log("Room Created :			", roomId);
}

RoomBase::~RoomBase()
{
	GLogManager->Log("Room Destroyed :			", roomId);
}

void RoomBase::Init()
{
	state = RoomState::Running;
}

void RoomBase::Close()
{
	if (state != RoomState::Running) return;

	GRoomManager->RemoveRoom(static_pointer_cast<RoomBase>(shared_from_this()));

	state = RoomState::Closing;

	HandleClose();
}

void RoomBase::HandleClose()
{
	for (auto client = clients.begin(); client != clients.end(); client++)
		client->second->DoAsync(&ClientBase::Leave, string("CLOSING"));

	state = RoomState::Closed;
}

void RoomBase::Handle_C_LEAVE(shared_ptr<ClientBase>& client, Protocol::C_LEAVE& pkt) { client->DoAsync(&ClientBase::Leave, string("LEAVED")); }
void RoomBase::Handle_C_SET_NICKNAME(shared_ptr<ClientBase>& client, Protocol::C_SET_NICKNAME& pkt) { DoAsync(&RoomBase::SetNickname, client, pkt.nickname()); }
void RoomBase::Handle_C_GET_CLIENT(shared_ptr<ClientBase>& client, Protocol::C_GET_CLIENT& pkt) { DoAsync(&RoomBase::GetClient, client); }
void RoomBase::Handle_C_CHAT(shared_ptr<ClientBase>& client, Protocol::C_CHAT& pkt) { DoAsync(&RoomBase::HandleChat, client, pkt.chat()); }
void RoomBase::Handle_C_WILDCARD(shared_ptr<ClientBase>& _client, Protocol::C_WILDCARD& pkt)
{
	Protocol::S_WILDCARD res;

	res.set_code(pkt.code());
	res.set_allocated_data(pkt.release_data());
	res.set_sender(_client->clientId);

	switch (pkt.type())
	{
	case 1:
	{
		Broadcast(PacketManager::MakeSendBuffer(res));
		break;
	}
	case 2:
	{
		auto sendBuffer = PacketManager::MakeSendBuffer(res);

		for (auto& [clientId, client] : clients)
		{
			if (clientId == _client->clientId)
				continue;

			client->Send(sendBuffer);
		}

		break;
	}
	case 3:
	{
		auto client = clients.find(pkt.receiver());
		if (client == clients.end())
			return;

		client->second->Send(PacketManager::MakeSendBuffer(res));

		break;
	}
	}
}

void RoomBase::Handle_C_WILDCARD_MAP(shared_ptr<ClientBase>& _client, Protocol::C_WILDCARD_MAP& pkt)
{
	Protocol::S_WILDCARD_MAP res;

	res.set_code(pkt.code());
	*res.mutable_data() = pkt.data();
	res.set_sender(_client->clientId);
	
	switch (pkt.type())
	{
	case 1:
	{
		Broadcast(PacketManager::MakeSendBuffer(res));
		break;
	}
	case 2:
	{
		auto sendBuffer = PacketManager::MakeSendBuffer(res);

		for (auto& [clientId, client] : clients)
		{
			if (clientId == _client->clientId)
				continue;

			client->Send(sendBuffer);
		}

		break;
	}
	case 3:
	{
		auto client = clients.find(pkt.receiver());
		if (client == clients.end())
			return;

		client->second->Send(PacketManager::MakeSendBuffer(res));

		break;
	}
	}
}

void RoomBase::Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt)
{
	if (state != RoomState::Running) return;

	auto [isEnterSucceed, errorCode] = HandleEnter(pkt);
	if (isEnterSucceed)
	{
		auto client = MakeClient(pkt.clientid(), pkt.sessionid());

		if (client == nullptr)
		{
			session->Disconnect();
			return;
		}

		client->session = session;
		session->owner = client;

		SetDefaultClientData(client);
		SetClientData(client);

		OnEnterSuccess(client);
	}
	else
	{
		Protocol::S_ENTER res;
		res.set_result(errorCode);
		session->Send(PacketManager::MakeSendBuffer(res));
		session->Disconnect();
	}
}

void RoomBase::Leave(shared_ptr<ClientBase> _client)
{
	if (state != RoomState::Running) return;

	auto client = clients.find(_client->clientId);
	if (client == clients.end())
		return;

	clients.erase(client);

	Protocol::S_REMOVE_CLIENT removeClient;
	removeClient.add_clientids(_client->clientId);
	Broadcast(PacketManager::MakeSendBuffer(removeClient));

	if (clients.size() == 0)
		Close();
}

void RoomBase::SetNickname(shared_ptr<ClientBase> _client, string nickname)
{
	if (state != RoomState::Running) return;

	auto client = clients.find(_client->clientId);
	if (client == clients.end())
		return;

	client->second->nickname = nickname;

	Protocol::S_SET_NICKNAME setNickname;
	setNickname.set_success(true);
	_client->Send(PacketManager::MakeSendBuffer(setNickname));
	
	Protocol::S_SET_NICKNAME_NOTICE setNicknameNotice;
	setNicknameNotice.set_clientid(client->second->clientId);
	setNicknameNotice.set_nickname(nickname);
	Broadcast(PacketManager::MakeSendBuffer(setNicknameNotice));
}

void RoomBase::GetClient(shared_ptr<ClientBase> _client)
{
	if (state != RoomState::Running) return;

	Protocol::S_ADD_CLIENT res;

	for (auto& client : clients)
	{
		auto clientInfo = res.add_clientinfos();
		clientInfo->set_clientid(client.second->clientId);
		clientInfo->set_nickname(client.second->nickname);
		clientInfo->set_statemessage(client.second->stateMessage);
	}

	if (res.clientinfos_size() > 0)
	{
		auto sendBuffer = PacketManager::MakeSendBuffer(res);
		_client->Send(sendBuffer);
	}
}

void RoomBase::HandleChat(shared_ptr<ClientBase> client, string chat)
{
	Protocol::S_CHAT res;
	res.set_clientid(client->clientId);
	res.set_chat(chat);
	Broadcast(PacketManager::MakeSendBuffer(res));
}

void RoomBase::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	for (auto client = clients.begin(); client != clients.end(); client++)
		client->second->Send(sendBuffer);
}

shared_ptr<ClientBase> RoomBase::MakeClient(string clientId, int sessionId)
{
	return GClientManager->MakeCilent<ClientBase>(clientId, sessionId, static_pointer_cast<RoomBase>(shared_from_this()));
}

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

void RoomBase::SetDefaultClientData(shared_ptr<ClientBase> client)
{
	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* res;

	driver = get_driver_instance();

	con = driver->connect(
		DBDomain,
		DBUsername,
		DBPassword
	);
	con->setSchema(DBSchema);
	stmt = con->createStatement();
	stmt->execute("SET NAMES 'utf8mb4'");

	pstmt = con->prepareStatement("SELECT nickname, stateMessage FROM member WHERE memberCode = ?");
	pstmt->setString(1, client->clientId);
	res = pstmt->executeQuery();

	while (res->next())
	{
		client->nickname = res->getString(1);
		client->stateMessage = res->getString(2);
	}

	con->close();

	delete res;
	delete stmt;
	delete pstmt;
	delete con;
}

void RoomBase::OnEnterSuccess(shared_ptr<ClientBase> client)
{
	clients.insert({ client->clientId, client });

	Protocol::S_ENTER res;
	res.set_result("SUCCESS");
	client->Send(PacketManager::MakeSendBuffer(res));

	Protocol::S_ADD_CLIENT addClient;
	auto clientInfo = addClient.add_clientinfos();
	clientInfo->set_clientid(client->clientId);
	clientInfo->set_nickname(client->nickname);
	clientInfo->set_statemessage(client->stateMessage);
	Broadcast(PacketManager::MakeSendBuffer(addClient));
}
