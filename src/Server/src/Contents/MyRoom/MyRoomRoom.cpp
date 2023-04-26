#include "MyRoomRoom.h"

#include "../../Session/GameSession.h"
#include "../../PacketManager.h"
#include "../Game/GameClient.h"
#include "../ClientManager.h"
#include "../RoomManager.h"

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

void MyRoomRoom::Init()
{
	state = RoomState::Running;
	
	roomInfo["roomId"] = roomId;
	roomInfo["ip"] = localHostIp;
	roomInfo["port"] = tcpPort;

	GRoomManager->IndexRoom(static_pointer_cast<RoomBase>(shared_from_this()));

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
	res = stmt->executeQuery("SELECT myRoomStateType FROM member WHERE memberCode = '" + ownerId + "'");

	isShutdown = res->getInt(1) == 4;

	delete res;
	delete stmt;
	delete con;

	this->DoTimer(30000, std::function<void()>(
		[this]() {
			if (this->state != RoomState::Running)
				return;

			if (this->clients.size() == 0)
				Close();
		}
	));
}

void MyRoomRoom::Handle_C_MYROOM_START_EDIT(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_START_EDIT& pkt) { DoAsync(&MyRoomRoom::HandleStartEdit, client); }
void MyRoomRoom::Handle_C_MYROOM_END_EDIT(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_END_EDIT& pkt) { DoAsync(&MyRoomRoom::HandleEndEdit, client, pkt.ischanged()); }
void MyRoomRoom::Handle_C_MYROOM_KICK(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_KICK& pkt) { DoAsync(&MyRoomRoom::Kick, client, pkt.clientid()); }
void MyRoomRoom::Handle_C_MYROOM_SHUTDOWN(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_SHUTDOWN& pkt) { DoAsync(&MyRoomRoom::HandleShutdown, client, pkt.isshutdown()); }

void MyRoomRoom::Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt)
{
	if (state != RoomState::Running) return;

	Protocol::S_ENTER res;

	{
		auto client = clients.find(pkt.clientid());
		if (client != clients.end())
		{
			client->second->DoAsync(&ClientBase::Leave, string("DUPLICATED"));
			DoTimer(1000, &GameRoom::Enter, session, pkt);
			return;
		}
	}

	if (isShutdown && pkt.clientid() != ownerId)
	{
		res.set_result("ROON_IS_SHUTDOWN");
		session->Send(PacketManager::MakeSendBuffer(res));
		session->Disconnect();
		return;
	}

	//TODO : blacklist ±¸Çö

	if (clients.size() >= maxPlayerNumber)
	{
		res.set_result("ROOM_IS_FULL");
		session->Send(PacketManager::MakeSendBuffer(res));
		session->Disconnect();
		return;
	}

	auto client = static_pointer_cast<GameClient>(GClientManager->MakeCilent<GameClient>(session, pkt.clientid(), pkt.nickname(), static_pointer_cast<RoomBase>(shared_from_this())));

	clients.insert({ pkt.clientid(), client });

	res.set_result("SUCCESS");
	session->Send(PacketManager::MakeSendBuffer(res));

	Protocol::S_ADD_CLIENT addClient;
	auto clientInfo = addClient.add_clientinfos();
	clientInfo->set_clientid(pkt.clientid());
	clientInfo->set_nickname(pkt.nickname());
	Broadcast(PacketManager::MakeSendBuffer(addClient));
}

void MyRoomRoom::HandleStartEdit(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;
	if (client->clientId != ownerId) return;

	Protocol::S_MYROOM_START_EDIT res;
	Broadcast(PacketManager::MakeSendBuffer(res));
}

void MyRoomRoom::HandleEndEdit(shared_ptr<ClientBase> client, bool isChanged)
{
	if (state != RoomState::Running) return;
	if (client->clientId != ownerId) return;

	Protocol::S_MYROOM_END_EDIT res;
	res.set_ischanged(isChanged);
	Broadcast(PacketManager::MakeSendBuffer(res));
}

void MyRoomRoom::Kick(shared_ptr<ClientBase> client, string kickedClientId)
{
	if (state != RoomState::Running) return;
	if (client->clientId != ownerId) return;

	Protocol::S_MYROOM_KICK res;

	auto kickedClient = clients.find(kickedClientId);
	if (kickedClient == clients.end())
	{
		res.set_success(false);
		client->Send(PacketManager::MakeSendBuffer(res));
		return;
	}

	kickedClient->second->DoAsync(&ClientBase::Leave, string("Kicked"));

	res.set_success(true);
	client->Send(PacketManager::MakeSendBuffer(res));
}

void MyRoomRoom::HandleShutdown(shared_ptr<ClientBase> client, bool isShutdown)
{
	if (state != RoomState::Running) return;
	if (client->clientId != ownerId) return;

	this->isShutdown = isShutdown;

	Protocol::S_MYROOM_SHUTDOWN res;
	res.set_isshutdown(isShutdown);
	Broadcast(PacketManager::MakeSendBuffer(res));
}
