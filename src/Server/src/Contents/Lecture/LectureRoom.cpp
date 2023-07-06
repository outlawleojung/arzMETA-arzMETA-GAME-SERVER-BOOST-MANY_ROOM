#include "LectureRoom.h"

#include "LectureClient.h"
#include "../../Session/GameSession.h"
#include "../../PacketManager.h"

#include "../RoomManager.h"

#include "../ClientManager.h"

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

LectureRoom::LectureRoom()
	: isWaitingRoom(false)
	, isPassword(false)
	, isAdvertising(false)
	, isShutdown(false)
	, passedTime(0)
	, topicType(0)
	, currentHostId("")
	, currentObserver(0)
	, currentPlayerNumber(0)
	, maxObserverNumber(0)
{
	//disconnectedSessionWaitTime = 30000;
}

LectureRoom::~LectureRoom()
{
	
}

void LectureRoom::Init()
{
	state = RoomState::Running;

	roomInfo["roomId"] = roomId;
	roomInfo["roomName"] = roomName;
	roomInfo["description"] = description;
	roomInfo["spaceInfoId"] = spaceInfoId;
	roomInfo["topicType"] = topicType;
	roomInfo["thumbnail"] = thumbnail;
	roomInfo["roomCode"] = roomCode;
	roomInfo["personnel"] = maxPlayerNumber;
	roomInfo["currentPersonnel"] = 0;
	roomInfo["isPassword"] = isPassword;
	roomInfo["isAdvertising"] = isAdvertising;
	roomInfo["isShutdown"] = isShutdown;
	roomInfo["isObserver"] = (maxObserverNumber > 0) ? true : false;
	roomInfo["sceneName"] = sceneName;

	roomInfo["roomType"] = roomTypeToString(type);

	//나중에 삭제
	roomInfo["modeType"] = 2;

	this->DoTimer(30000, std::function<void()>(
		[this]() {
			if (this->state != RoomState::Running)
				return;

			if (this->clients.size() == 0)
				Close();
		}
	));
}

#include <soci/soci.h>

void LectureRoom::HandleClose()
{
	for (auto client = waitingClients.begin(); client != waitingClients.end(); client++)
		client->second->DoAsync(&ClientBase::Leave, string("CLOSING"));

	GameRoom::HandleClose();

	soci::session sql(*DBConnectionPool);
	int repeatDay = -1;

	sql << "SELECT repeatDay FROM memberofficereservationinfo WHERE roomCode = :roomCode",
		soci::use(roomCode), soci::into(repeatDay);

	if (repeatDay == 0)
	{
		sql << "DELETE FROM memberofficereservationinfo WHERE roomCode = :roomCode", soci::use(roomCode);
	}
}

void LectureRoom::Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) { DoAsync(&LectureRoom::Enter, session, pkt); }

void LectureRoom::Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<ClientBase>& client, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt) { DoAsync(&LectureRoom::InstantiateObject, client, pkt); }
void LectureRoom::Handle_C_INTERACTION_SET_ITEM(shared_ptr<ClientBase>& client, Protocol::C_INTERACTION_SET_ITEM& pkt) {}

void LectureRoom::Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_WAITING_LIST& pkt) { DoAsync(&LectureRoom::GetWaitingList, client); }
void LectureRoom::Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_ACCEPT_WAIT& pkt) { DoAsync(&LectureRoom::AcceptWait, client, pkt); } 
void LectureRoom::Handle_C_OFFICE_KICK(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_KICK& pkt) { DoAsync(&LectureRoom::Kick, client, pkt.clientid()); }
void LectureRoom::Handle_C_OFFICE_GET_PERMISSION(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_PERMISSION& pkt) { DoAsync(&LectureRoom::GetPermission, client, pkt.clientid()); }
void LectureRoom::Handle_C_OFFICE_GET_PERMISSION_ALL(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_PERMISSION_ALL& pkt) { DoAsync(&LectureRoom::GetPermissionAll, client); }
void LectureRoom::Handle_C_OFFICE_SET_PERMISSION(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_SET_PERMISSION& pkt) { DoAsync(&LectureRoom::SetPermission, client, pkt); }
void LectureRoom::Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_SET_ROOM_INFO& pkt) { DoAsync(&LectureRoom::SetRoomInfo, client, pkt); }
void LectureRoom::Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_ROOM_INFO& pkt) { DoAsync(&LectureRoom::GetRoomInfo, client); }
void LectureRoom::Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_VIDEO_STREAM& pkt) { DoAsync(&LectureRoom::HandleVideoStream, pkt.url(), pkt.volume(), pkt.time(), pkt.play(), pkt.seek(), pkt.mediaplayerstate()); }
void LectureRoom::Handle_C_OFFICE_SHARE(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_SHARE& pkt) { DoAsync(&LectureRoom::HandleShare, session, pkt.isshared(), pkt.userid()); }

void LectureRoom::Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt)
{
	if (state != RoomState::Running) return;

	pair<bool, string> enterResult;

	if (currentHostId.empty() && creatorId == pkt.clientid())
	{
		enterResult = { true, "SUCCESS" };
	}
	else if (clients.size() == 0)
	{
		enterResult = { false, "HOST_NOT_ENTERED" };
	}
	else if (isShutdown)
	{
		enterResult = { false, "ROOM_IS_SHUTDONW" };
	}
	else if (isPassword && password != pkt.password())
	{
		enterResult = { false, "PASSWORD_FAIL" };
	}
	else if (isWaitingRoom)
	{
		enterResult = { true, "WAITING" };
	}
	else if ((!pkt.isobserver() && currentPlayerNumber >= maxPlayerNumber) || (pkt.isobserver() && currentObserver >= maxObserverNumber))
	{
		enterResult = { true, "ROOM_IS_FULL" };
	}
	else
	{
		enterResult = { true, "SUCCESS" };
	}

	Protocol::S_ENTER res;
	res.set_result(enterResult.second);

	if (!enterResult.first)
	{
		session->Send(PacketManager::MakeSendBuffer(res));
		return;
	}

	auto _client = GClientManager->MakeCilent<LectureClient>(pkt.clientid(), pkt.sessionid(), static_pointer_cast<RoomBase>(shared_from_this()));
	if (_client == nullptr)
	{
		session->Disconnect();
		return;
	}

	auto client = static_pointer_cast<LectureClient>(_client);

	client->session = session;
	session->owner = client;

	SetDefaultClientData(client);

	if (currentHostId.empty() && creatorId == client->clientId)
	{
		client->data.type = LectureRoomUserType::Host;
		client->data.chatPermission = true;
		client->data.videoPermission = true;
		client->data.voicePermission = true;
		client->data.screenPermission = true;
	}
	else
	{
		if (pkt.isobserver())
		{
			client->data.type = LectureRoomUserType::Observer;
			client->data.chatPermission = false;
			client->data.videoPermission = false;
			client->data.voicePermission = false;
			client->data.screenPermission = false;
		}
		else
		{
			client->data.type = LectureRoomUserType::Audience;
			client->data.chatPermission = true;
			client->data.videoPermission = false;
			client->data.voicePermission = false;
			client->data.screenPermission = false;
		}
	}

	if (client->data.type == LectureRoomUserType::Host)
	{
		clients.insert({ client->clientId, client });
		roomInfo["currentPersonnel"] = ++currentPlayerNumber;

		res.set_result("SUCCESS");
		client->Send(PacketManager::MakeSendBuffer(res));

		Protocol::S_ADD_CLIENT addClient;
		auto clientInfo = addClient.add_clientinfos();
		clientInfo->set_clientid(client->clientId);
		clientInfo->set_nickname(client->nickname);
		clientInfo->set_statemessage(client->stateMessage);
		Broadcast(PacketManager::MakeSendBuffer(addClient));

		currentHostId = client->clientId;
		roomInfo["hostName"] = client->nickname;

		createdTimeString = GetCurrentTimeString();
		roomInfo["createdTime"] = GetCurrentTimeString();

		DoTimer(60000, &LectureRoom::Countdown);

		GRoomManager->IndexRoom(static_pointer_cast<RoomBase>(shared_from_this()));
	}
	else if (isWaitingRoom)
	{
		waitingClients.insert({ client->clientId, client });

		res.set_result("WAITING");
		client->Send(PacketManager::MakeSendBuffer(res));

		Protocol::S_OFFICE_ADD_WAITING_CLIENT addWaitingClient;
		auto waitingClient = addWaitingClient.add_clients();
		waitingClient->set_isobserver(pkt.isobserver());
		waitingClient->set_clientid(client->clientId);
		waitingClient->set_nickname(client->nickname);
		clients.find(currentHostId)->second->session->Send(PacketManager::MakeSendBuffer(addWaitingClient));
	}
	else
	{
		if (pkt.isobserver())
		{
			currentObserver++;
		}
		else
		{
			roomInfo["currentPersonnel"] = ++currentPlayerNumber;
		}

		clients.insert({ client->clientId, client });

		res.set_result("SUCCESS");
		client->Send(PacketManager::MakeSendBuffer(res));

		Protocol::S_ADD_CLIENT addClient;
		auto clientInfo = addClient.add_clientinfos();
		clientInfo->set_clientid(client->clientId);
		clientInfo->set_nickname(client->nickname);
		clientInfo->set_statemessage(client->stateMessage);
		Broadcast(PacketManager::MakeSendBuffer(addClient));
	}
}

void LectureRoom::Leave(shared_ptr<ClientBase> _client)
{
	if (state != RoomState::Running) return;

	auto client = static_pointer_cast<LectureClient>(_client);
	
	//대기열 먼저 확인, 존재했으면 지우고 호스트에게 알림
	{
		auto waitingClient = waitingClients.find(client->clientId);
		if (waitingClient != waitingClients.end())
		{
			Protocol::S_OFFICE_REMOVE_WAITING_CLIENT removedWaitingClients;
			auto removedWaitingClient = removedWaitingClients.add_clients();
			removedWaitingClient->set_clientid(waitingClient->second->clientId);
			removedWaitingClient->set_isobserver(client->data.type == LectureRoomUserType::Observer);
			clients.find(currentHostId)->second->session->Send(PacketManager::MakeSendBuffer(removedWaitingClients));

			waitingClients.erase(waitingClient);
			return;
		}
	}

	GameRoom::Leave(_client);

	if (client->data.type != LectureRoomUserType::Observer)
	{
		roomInfo["currentPersonnel"] = --currentPlayerNumber;
	}
	else
	{
		currentObserver--;
	}

	if (_client->clientId == currentHostId)
		Close();
}

void LectureRoom::GetWaitingList(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	Protocol::S_OFFICE_ADD_WAITING_CLIENT waitingClientsPkt;
	for (auto waitingClient = waitingClients.begin(); waitingClient != waitingClients.end(); waitingClient++)
	{
		auto waitingClientInfo = waitingClientsPkt.add_clients();
		waitingClientInfo->set_isobserver(waitingClient->second->data.type == LectureRoomUserType::Observer);
		waitingClientInfo->set_clientid(waitingClient->second->clientId);
		waitingClientInfo->set_nickname(waitingClient->second->nickname);
	}

	client->Send(PacketManager::MakeSendBuffer(waitingClientsPkt));
}

void LectureRoom::AcceptWait(shared_ptr<ClientBase> _client, Protocol::C_OFFICE_ACCEPT_WAIT pkt)
{
	if (state != RoomState::Running) return;

	if (_client->clientId != currentHostId) return;

	Protocol::S_OFFICE_ACCEPT_WAIT acceptWait;

	int observerCount = 0;
	int nonObserverCount = 0;

	//수락하려는 클라이언트가 없는 경우 실패 처리
	for (int i = 0; i < pkt.clientid_size(); i++)
	{
		auto waitingClient = waitingClients.find(pkt.clientid()[i]);
		if (waitingClient == waitingClients.end())
		{
			acceptWait.set_success(false);
			//set result as "no client with clientid ~"
			_client->Send(PacketManager::MakeSendBuffer(acceptWait));
			return;
		}

		if (waitingClient->second->data.type == LectureRoomUserType::Observer)
		{
			observerCount++;
		}
		else
		{
			nonObserverCount++;
		}
	}

	//인원이 꽉 찬 상태에서 수락하려고 하는 경우 실패 처리
	if (pkt.isaccepted()
		&& (currentObserver + observerCount > maxObserverNumber || currentPlayerNumber + nonObserverCount > maxPlayerNumber))
	{
		acceptWait.set_success(false);
		//set result as "room is full"
		_client->Send(PacketManager::MakeSendBuffer(acceptWait));
		return;
	}

	//이외의 경우 성공, 호스트에게 관련 메시지 전송
	acceptWait.set_success(true);
	_client->Send(PacketManager::MakeSendBuffer(acceptWait));

	for (int i = 0; i < pkt.clientid_size(); i++)
	{
		auto waitingClient = waitingClients.find(pkt.clientid()[i]);

		Protocol::S_OFFICE_REMOVE_WAITING_CLIENT removeWaitingClient;
		auto removedWaitingClient = removeWaitingClient.add_clients();
		removedWaitingClient->set_clientid(waitingClient->second->clientId);
		removedWaitingClient->set_isobserver(waitingClient->second->data.type == LectureRoomUserType::Observer);
		_client->Send(PacketManager::MakeSendBuffer(removeWaitingClient));

		//대상 클라이언트에게 성공/실패 메시지 전송
		Protocol::S_OFFICE_ACCEPT_WAIT_NOTICE acceptWaitNotice;
		acceptWaitNotice.set_isaccepted(pkt.isaccepted());
		waitingClient->second->session->Send(PacketManager::MakeSendBuffer(acceptWaitNotice));

		//입장 허락이었을 경우의 처리, enter 시의 처리와 동일
		if (pkt.isaccepted())
		{
			if (waitingClient->second->data.type == LectureRoomUserType::Observer)
			{
				waitingClient->second->data.chatPermission = false;
				waitingClient->second->data.videoPermission = false;
				waitingClient->second->data.voicePermission = false;
				waitingClient->second->data.screenPermission = false;
				waitingClient->second->data.movePermission = false;

				currentObserver++;
			}
			else
			{
				waitingClient->second->data.chatPermission = true;
				waitingClient->second->data.videoPermission = false;
				waitingClient->second->data.voicePermission = false;
				waitingClient->second->data.screenPermission = false;
				waitingClient->second->data.movePermission = false;

				roomInfo["currentPersonnel"] = ++currentPlayerNumber;
			}

			clients.insert({ waitingClient->second->clientId, waitingClient->second });

			Protocol::S_ENTER res;
			res.set_result("SUCCESS");
			waitingClient->second->Send(PacketManager::MakeSendBuffer(res));

			Protocol::S_ADD_CLIENT addClient;
			auto clientInfo = addClient.add_clientinfos();
			clientInfo->set_clientid(waitingClient->second->clientId);
			clientInfo->set_nickname(waitingClient->second->nickname);
			clientInfo->set_statemessage(waitingClient->second->stateMessage);
			Broadcast(PacketManager::MakeSendBuffer(addClient));

			waitingClients.erase(waitingClient);
		}
		else
		{
			//Protocol::S_ENTER res;
			//res.set_result("WAITING_REJECTED");
			//waitingClient->second->Send(PacketManager::MakeSendBuffer(res));

			waitingClient->second->DoAsync(&ClientBase::Leave, string("WAITING_REJECTED"));
		}
	}
}

void LectureRoom::Kick(shared_ptr<ClientBase> client, string clientId)
{
	if (state != RoomState::Running) return;
	if (client->clientId != currentHostId) return;
	if (clientId == currentHostId) return;

	Protocol::S_OFFICE_KICK res;

	//존재하는지 확인
	auto kickedClient = clients.find(clientId);
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

void LectureRoom::GetPermission(shared_ptr<ClientBase> _client, string clientId)
{
	if (state != RoomState::Running) return;

	auto client = clients.find(clientId);
	if (client == clients.end())
		return;

	Protocol::S_OFFICE_GET_PERMISSION getPermission;
	auto permission = getPermission.mutable_permission();

	auto oClient = static_pointer_cast<LectureClient>(client->second);

	permission->set_clientid(oClient->clientId);
	permission->set_screenpermission(oClient->data.screenPermission);
	permission->set_chatpermission(oClient->data.chatPermission);
	permission->set_voicepermission(oClient->data.voicePermission);
	permission->set_videopermission(oClient->data.videoPermission);
	permission->set_authority(static_cast<int>(oClient->data.type));

	_client->Send(PacketManager::MakeSendBuffer(getPermission));
}

void LectureRoom::GetPermissionAll(shared_ptr<ClientBase> _client)
{
	if (state != RoomState::Running) return;

	Protocol::S_OFFICE_GET_PERMISSION_ALL getPermissionAll;

	for (auto client = clients.begin(); client != clients.end(); client++)
	{
		auto permission = getPermissionAll.add_permissions();

		auto oClient = static_pointer_cast<LectureClient>(client->second);

		permission->set_clientid(oClient->clientId);
		permission->set_screenpermission(oClient->data.screenPermission);
		permission->set_chatpermission(oClient->data.chatPermission);
		permission->set_voicepermission(oClient->data.voicePermission);
		permission->set_videopermission(oClient->data.videoPermission);
		permission->set_authority(static_cast<int>(oClient->data.type));
	}

	if (getPermissionAll.permissions_size() > 0)
	{
		auto sendBuffer = PacketManager::MakeSendBuffer(getPermissionAll);
		_client->Send(sendBuffer);
	}
}

void LectureRoom::SetPermission(shared_ptr<ClientBase> _client, Protocol::C_OFFICE_SET_PERMISSION pkt)
{
	if (state != RoomState::Running) return;

	if (_client->clientId != currentHostId) return;

	pair<bool, string> result = { true, "SUCCESS" };
	int observerCount = 0;
	int nonObserverCount = 0;

	map<string, LectureRoomUserData> newUserData;
	for (int i = 0; i < pkt.permissions_size(); i++)
	{
		if (clients.find(pkt.permissions()[i].clientid()) == clients.end())
		{
			result = { false, "WRONG_CLIENT_ID" };
			goto SET_PERMISSION_LOGIC;
		}

		LectureRoomUserData userData;
		userData.type = static_cast<LectureRoomUserType>(pkt.permissions()[i].authority());
		userData.screenPermission = pkt.permissions()[i].screenpermission();
		userData.chatPermission = pkt.permissions()[i].chatpermission();
		userData.videoPermission = pkt.permissions()[i].videopermission();
		userData.voicePermission = pkt.permissions()[i].voicepermission();
		newUserData.insert({ pkt.permissions()[i].clientid(), userData });
	}

	for (auto& [clientId, client] : clients)
	{
		if (newUserData.find(clientId) != newUserData.end())
			continue;

		LectureRoomUserData userData(static_pointer_cast<LectureClient>(client)->data);
		newUserData.insert({ client->clientId, userData });
	}

	// 요청 검증
	// 호스트가 하나인지, 해당 호스트가 접속해 있는지 확인
	// screen permission 이 하나인지 확인
	// 관전자와 참가자의 수가 맞는지 확인

	{
		int hostCount = 0;
		int screenCount = 0;

		for (auto& [clientId, userData] : newUserData)
		{
			if (userData.type == LectureRoomUserType::Host)
			{
				hostCount++;
				if (hostCount > 1)
				{
					result = { false, "TOO_MANY_HOST" };
					goto SET_PERMISSION_LOGIC;
				}
			}

			if (userData.screenPermission)
			{
				screenCount++;
				if (screenCount > 1)
				{
					result = { false, "TOO_MANY_SCREEN_PERMISSION" };
					goto SET_PERMISSION_LOGIC;
				}
			}

			if (userData.type == LectureRoomUserType::Observer)
			{
				observerCount++;
				if (observerCount > maxObserverNumber)
				{
					result = { false, "TOO_MANY_OBSERVER" };
					goto SET_PERMISSION_LOGIC;
				}
			}
			else
			{
				nonObserverCount++;
				if (nonObserverCount > maxPlayerNumber)
				{
					result = { false, "TOO_MANY_NON_OBSERVER" };
					goto SET_PERMISSION_LOGIC;
				}
			}
		}

		if (hostCount == 0)
		{
			result = { false, "NO_HOST" };
			goto SET_PERMISSION_LOGIC;
		}
	}

SET_PERMISSION_LOGIC:

	Protocol::S_OFFICE_SET_PERMISSION res;
	res.set_code(result.second);
	_client->Send(PacketManager::MakeSendBuffer(res));

	if (!result.first)
		return;

	currentObserver = observerCount;
	currentPlayerNumber = nonObserverCount;
	roomInfo["currentPersonnel"] = nonObserverCount;

	for (auto& [clientId, userData] : newUserData)
	{
		auto client = static_pointer_cast<LectureClient>(clients.find(clientId)->second);
		if (client->data == userData) continue;

		if (client->data.type != LectureRoomUserType::Observer && userData.type == LectureRoomUserType::Observer)
		{
			client->data.type = LectureRoomUserType::Observer;
			client->data.chatPermission = false;
			client->data.screenPermission = false;
			client->data.videoPermission = false;
			client->data.voicePermission = false;
			client->data.movePermission = false;
		}
		else
		{
			client->data = userData;
		}

		if (client->data.type == LectureRoomUserType::Host)
		{
			currentHostId = clientId;
			roomInfo["hostName"] = client->nickname;
		}

		Protocol::S_OFFICE_GET_PERMISSION permissionNotice;

		auto permission = permissionNotice.mutable_permission();

		permission->set_clientid(client->clientId);
		permission->set_screenpermission(client->data.screenPermission);
		permission->set_chatpermission(client->data.chatPermission);
		permission->set_voicepermission(client->data.voicePermission);
		permission->set_videopermission(client->data.videoPermission);
		permission->set_authority(static_cast<int>(client->data.type));

		client->Send(PacketManager::MakeSendBuffer(permissionNotice));

		if (client->data.type == LectureRoomUserType::Observer)
		{
			RemoveObject(client);
		}
	}
}

void LectureRoom::GetRoomInfo(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	Protocol::S_OFFICE_GET_ROOM_INFO roomInfo;

	roomInfo.set_roomname(roomName);
	roomInfo.set_description(description);
	roomInfo.set_topictype(topicType);
	roomInfo.set_password(password);
	roomInfo.set_spaceinfoid(spaceInfoId);
	roomInfo.set_personnel(maxPlayerNumber);
	roomInfo.set_currentpersonnel(currentPlayerNumber);
	roomInfo.set_isadvertising(isAdvertising);
	roomInfo.set_thumbnail(thumbnail);
	roomInfo.set_iswaitingroom(isWaitingRoom);
	roomInfo.set_currentwaiting(waitingClients.size());
	roomInfo.set_isshutdown(isShutdown);
	roomInfo.set_starttime(createdTimeString);
	roomInfo.set_runningtime(runningTime);
	roomInfo.set_passedtime(passedTime);
	roomInfo.set_roomcode(roomCode);
	roomInfo.set_observer(maxObserverNumber);
	roomInfo.set_currentobserver(currentObserver);
		
	auto host = clients.find(currentHostId);
	if (host != clients.end())
		roomInfo.set_hostnickname(host->second->nickname);

	client->Send(PacketManager::MakeSendBuffer(roomInfo));
}

void LectureRoom::SetRoomInfo(shared_ptr<ClientBase> client, Protocol::C_OFFICE_SET_ROOM_INFO pkt)
{
	if (state != RoomState::Running) return;

	if (client->clientId != currentHostId) return;

	maxPlayerNumber = pkt.personnel();
	password = pkt.password();
	if (password == "")
		isPassword = false;
	else
		isPassword = true;
	isAdvertising = pkt.isadvertising();
	isShutdown = pkt.isshutdown();
	isWaitingRoom = pkt.iswaitingroom();
	runningTime = pkt.runningtime();
	thumbnail = pkt.thumbnail();
	maxObserverNumber = pkt.observer();
	if(maxObserverNumber > 0)
		roomInfo["isObserver"] = true;
	else
		roomInfo["isObserver"] = false;

	roomInfo["personnel"] = maxPlayerNumber;
	roomInfo["isPassword"] = isPassword;
	roomInfo["isAdvertising"] = isAdvertising;
	roomInfo["isShutdown"] = isShutdown;
	roomInfo["thumbnail"] = thumbnail;

	Protocol::S_OFFICE_SET_ROOM_INFO roomInfo;
	roomInfo.set_success(true);
	Broadcast(PacketManager::MakeSendBuffer(roomInfo));
}

void LectureRoom::HandleVideoStream(string url, float volume, float time, bool play, bool seek, int mediaplayerstate)
{
	if (state != RoomState::Running) return;

	Protocol::S_OFFICE_VIDEO_STREAM videoStream;

	videoStream.set_url(url);
	videoStream.set_volume(volume);
	videoStream.set_time(time);
	videoStream.set_play(play);
	videoStream.set_seek(seek);
	videoStream.set_mediaplayerstate(mediaplayerstate);

	auto sendBuffer = PacketManager::MakeSendBuffer(videoStream);
	this->DoAsync(&LectureRoom::Broadcast, sendBuffer);
}

void LectureRoom::HandleShare(shared_ptr<ClientBase> session, bool isShared, int userId)
{
	if (state != RoomState::Running) return;

	Protocol::S_OFFICE_SHARE share;
	share.set_isshared(isShared);
	share.set_userid(userId);

	auto sendBuffer = PacketManager::MakeSendBuffer(share);
	for (auto client = clients.begin(); client != clients.end(); client++)
	{
		if (client->second->clientId == session->clientId)
			continue;

		client->second->Send(sendBuffer);
	}
}

void LectureRoom::InstantiateObject(shared_ptr<ClientBase> _client, Protocol::C_BASE_INSTANTIATE_OBJECT pkt)
{
	if (state != RoomState::Running) return;

	auto client = clients.find(_client->clientId);
	if (client == clients.end())
		return;

	auto oClient = static_pointer_cast<LectureClient>(client->second);

	if (oClient->data.type == LectureRoomUserType::Observer)
		return;

	GameRoom::Handle_C_BASE_INSTANTIATE_OBJECT(_client, pkt);
}

void LectureRoom::SetState(shared_ptr<ClientBase> _client, Protocol::C_INTERACTION_SET_ITEM pkt)
{
	if (state != RoomState::Running) return;

	auto client = clients.find(_client->clientId);
	if (client == clients.end())
		return;

	auto oClient = static_pointer_cast<LectureClient>(client->second);

	if (oClient->data.type == LectureRoomUserType::Observer)
		return;

	GameRoom::Handle_C_INTERACTION_SET_ITEM(_client, pkt);
}

void LectureRoom::RemoveState(shared_ptr<ClientBase> _client, Protocol::C_INTERACTION_REMOVE_ITEM pkt)
{
	if (state != RoomState::Running) return;

	auto client = clients.find(_client->clientId);
	if (client == clients.end())
		return;

	auto oClient = static_pointer_cast<LectureClient>(client->second);

	if (oClient->data.type == LectureRoomUserType::Observer)
		return;

	GameRoom::Handle_C_INTERACTION_REMOVE_ITEM(_client, pkt);
}

void LectureRoom::Countdown()
{
	if (state != RoomState::Running) return;

	if (passedTime - runningTime >= 0)
	{
		Close();
		return;
	}

	passedTime++;

	this->DoTimer(60000, &LectureRoom::Countdown);
}

//void LectureRoom::Broadcast(shared_ptr<SendBuffer> sendBuffer)
//{
//	RoomBase::Broadcast(sendBuffer);
//
//	for (const auto& [clientId, observer] : observers)
//		observer->Send(sendBuffer);
//}
