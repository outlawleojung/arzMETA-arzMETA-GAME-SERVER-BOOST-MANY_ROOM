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
	roomInfo["sceneName"] = sceneName;

	roomInfo["roomType"] = roomTypeToString(type);

	GRoomManager->IndexRoom(static_pointer_cast<RoomBase>(shared_from_this()));

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
	
	stmt = con->createStatement();
	stmt->execute("SET NAMES 'utf8mb4'");
	
	string ownerMemberId;

	{
		res = stmt->executeQuery("SELECT memberId, nickname, myRoomStateType FROM member WHERE memberCode = '" + ownerId + "'");

		while (res->next())
		{
			ownerMemberId = res->getString(1);
			ownerNickname = res->getString(2);
			isShutdown = res->getInt(3) == 4;
		}
	}
	
	{
		res = stmt->executeQuery("SELECT avatarPartsType, itemId FROM memberavatarinfo WHERE memberId = '" + ownerMemberId + "'");

		while (res->next())
			ownerAvatarInfo[res->getString(1)] = res->getString(2);
	}

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

void MyRoomRoom::Handle_C_MYROOM_GET_ROOMINFO(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_GET_ROOMINFO& pkt) { DoAsync(&MyRoomRoom::GetRoomInfo, client); }
void MyRoomRoom::Handle_C_MYROOM_START_EDIT(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_START_EDIT& pkt) { DoAsync(&MyRoomRoom::HandleStartEdit, client); }
void MyRoomRoom::Handle_C_MYROOM_END_EDIT(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_END_EDIT& pkt) { DoAsync(&MyRoomRoom::HandleEndEdit, client, pkt.ischanged()); }
void MyRoomRoom::Handle_C_MYROOM_KICK(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_KICK& pkt) { DoAsync(&MyRoomRoom::Kick, client, pkt.clientid()); }
void MyRoomRoom::Handle_C_MYROOM_SHUTDOWN(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_SHUTDOWN& pkt) { DoAsync(&MyRoomRoom::HandleShutdown, client, pkt.isshutdown()); }

pair<bool, string> MyRoomRoom::HandleEnter(const Protocol::C_ENTER& pkt)
{
	if (isShutdown && pkt.clientid() != ownerId)
		return { false, "ROON_IS_SHUTDOWN" };

	//TODO : blacklist ±¸Çö

	if (clients.size() >= maxPlayerNumber)
		return { false, "ROOM_IS_FULL" };

	return { true, "SUCCESS" };
}

void MyRoomRoom::GetRoomInfo(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	Protocol::S_MYROOM_GET_ROOMINFO res;

	res.set_ownerid(ownerId);
	res.set_ownernickname(ownerNickname);
	res.set_owneravatarinfo(ownerAvatarInfo.dump());
	res.set_isshutdown(isShutdown);
	client->Send(PacketManager::MakeSendBuffer(res));
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

	kickedClient->second->DoAsync(&ClientBase::Leave, string("KICKED"));

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
