#include "MeetingRoom.h"
#include "MeetingClient.h"
#include "../../Session/GameSession.h"
#include "../../PacketManager.h"

#include "../RoomManager.h"

#include "../ClientManager.h"

MeetingRoom::MeetingRoom()
	: isWaitingRoom(false)
	, isPassword(false)
	, isAdvertising(false)
	, isShutdown(false)
	, passedTime(0)
	, topicType(0)
	, currentHostId("")
{
	disconnectedSessionWaitTime = 10000;
}

void MeetingRoom::Init()
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
	roomInfo["sceneName"] = sceneName;

	//나중에 삭제
	roomInfo["observer"] = 0;
	roomInfo["modeType"] = 1;

	this->DoTimer(30000, std::function<void()>(
		[this]() {
			if (this->state != RoomState::Running)
				return;

			if (this->clients.size() == 0)
				Close();
		}
	));
}

void MeetingRoom::HandleClose()
{
	for (auto client = waitingClients.begin(); client != waitingClients.end(); client++)
		client->second->DoAsync(&ClientBase::Leave, string("CLOSING"));

	GameRoom::HandleClose();
}

void MeetingRoom::Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) { DoAsync(&MeetingRoom::Enter, session, pkt); }

void MeetingRoom::Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_WAITING_LIST& pkt) { DoAsync(&MeetingRoom::GetWaitingList, client); }
void MeetingRoom::Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_ACCEPT_WAIT& pkt) { DoAsync(&MeetingRoom::AcceptWait, client, pkt.clientid(), pkt.isaccepted()); }
void MeetingRoom::Handle_C_OFFICE_GET_HOST(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_HOST& pkt) { DoAsync(&MeetingRoom::GetHost, client); }
void MeetingRoom::Handle_C_OFFICE_BREAK(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_BREAK& pkt) { DoAsync(&MeetingRoom::Break, client); }
void MeetingRoom::Handle_C_OFFICE_KICK(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_KICK& pkt) { DoAsync(&MeetingRoom::Kick, client, pkt.clientid()); }
void MeetingRoom::Handle_C_OFFICE_GET_PERMISSION(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_PERMISSION& pkt) { DoAsync(&MeetingRoom::GetPermission, client, pkt.clientid()); }
void MeetingRoom::Handle_C_OFFICE_GET_PERMISSION_ALL(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_PERMISSION_ALL& pkt) { DoAsync(&MeetingRoom::GetPermissionAll, client); }
void MeetingRoom::Handle_C_OFFICE_SET_PERMISSION(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_SET_PERMISSION& pkt) { DoAsync(&MeetingRoom::SetPermission, client, pkt); }
void MeetingRoom::Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_SET_ROOM_INFO& pkt) { DoAsync(&MeetingRoom::SetRoomInfo, client, pkt); }
void MeetingRoom::Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_ROOM_INFO& pkt) { DoAsync(&MeetingRoom::GetRoomInfo, client); }
void MeetingRoom::Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_VIDEO_STREAM& pkt) { DoAsync(&MeetingRoom::HandleVideoStream, pkt.url(), pkt.volume(), pkt.time(), pkt.play(), pkt.seek(), pkt.mediaplayerstate()); }
void MeetingRoom::Handle_C_OFFICE_SHARE(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_SHARE& pkt) { DoAsync(&MeetingRoom::HandleShare, session, pkt.isshared(), pkt.userid()); }

void MeetingRoom::Leave(shared_ptr<ClientBase> _client)
{
	if (state != RoomState::Running) return;

	//대기열 먼저 확인, 존재했으면 지우고 호스트에게 알림
	{
		auto waitingClient = waitingClients.find(_client->clientId);
		if (waitingClient != waitingClients.end())
		{
			Protocol::S_OFFICE_REMOVE_WAITING_CLIENT removedWaitingClients;
			auto removedWaitingClient = removedWaitingClients.add_clients();
			removedWaitingClient->set_clientid(waitingClient->second->clientId);
			clients.find(currentHostId)->second->session->Send(PacketManager::MakeSendBuffer(removedWaitingClients));

			waitingClients.erase(waitingClient);
			return;
		}
	}

	GameRoom::Leave(_client);

	roomInfo["currentPersonnel"] = clients.size();

	if (_client->clientId == currentHostId)
		Close();
}

pair<bool, string> MeetingRoom::HandleEnter(const Protocol::C_ENTER& pkt)
{
	if (currentHostId.empty() && creatorId == pkt.clientid())
		return { true, "SUCCESS" };

	if (clients.size() == 0)
		return { false, "HOST_NOT_ENTERED" };

	if (isShutdown)
		return { false, "ROOM_IS_SHUTDONW" };

	if (isPassword && password != pkt.password())
		return { false, "PASSWORD_FAIL" };

	if (isWaitingRoom)
		return { true, "WAITING" };

	if (clients.size() >= maxPlayerNumber)
		return { true, "ROOM_IS_FULL" };

	return { true, "SUCCESS" };
}

void MeetingRoom::SetClientData(shared_ptr<ClientBase> _client)
{
	auto client = static_pointer_cast<MeetingClient>(_client);

	if (currentHostId.empty() && creatorId == client->clientId)
	{
		client->data.type = MeetingRoomUserType::Host;
		client->data.chatPermission = true;
		client->data.videoPermission = true;
		client->data.voicePermission = true;
		client->data.screenPermission = true;
	}
	else if (isWaitingRoom)
	{
		client->data.type = MeetingRoomUserType::Guest;
		client->data.chatPermission = false;
		client->data.videoPermission = false;
		client->data.voicePermission = false;
		client->data.screenPermission = false;
	}
	else
	{
		client->data.type = MeetingRoomUserType::Guest;
		client->data.chatPermission = true;
		client->data.videoPermission = false;
		client->data.voicePermission = false;
		client->data.screenPermission = false;
	}
}

void MeetingRoom::OnEnterSuccess(shared_ptr<ClientBase> _client)
{
	auto client = static_pointer_cast<MeetingClient>(_client);

	if (client->data.type == MeetingRoomUserType::Host)
	{
		clients.insert({ client->clientId, client });
		roomInfo["currentPersonnel"] = clients.size();

		Protocol::S_ENTER res;
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

		DoAsync(&MeetingRoom::Countdown);

		GRoomManager->IndexRoom(static_pointer_cast<RoomBase>(shared_from_this()));
	}
	else if (isWaitingRoom)
	{
		waitingClients.insert({ client->clientId, client });

		Protocol::S_ENTER res;
		res.set_result("WAITING");
		client->Send(PacketManager::MakeSendBuffer(res));

		Protocol::S_OFFICE_ADD_WAITING_CLIENT addWaitingClient;
		auto waitingClient = addWaitingClient.add_clients();
		waitingClient->set_clientid(client->clientId);
		waitingClient->set_nickname(client->nickname);
		clients.find(currentHostId)->second->session->Send(PacketManager::MakeSendBuffer(addWaitingClient));
	}
	else
	{
		clients.insert({ client->clientId, client });
		roomInfo["currentPersonnel"] = clients.size();

		Protocol::S_ENTER res;
		res.set_result("SUCCESS");
		client->Send(PacketManager::MakeSendBuffer(res));

		Protocol::S_ADD_CLIENT addClient;
		auto clientInfo = addClient.add_clientinfos();
		clientInfo->set_clientid(client->clientId);
		clientInfo->set_nickname(client->clientId);
		clientInfo->set_statemessage(client->stateMessage);
		Broadcast(PacketManager::MakeSendBuffer(addClient));
	}
}

shared_ptr<ClientBase> MeetingRoom::MakeClient(string clientId, int sessionId)
{
	auto client = GClientManager->MakeCilent<MeetingClient>(clientId, sessionId, static_pointer_cast<RoomBase>(shared_from_this()));

	static_pointer_cast<MeetingClient>(client)->data.type = MeetingRoomUserType::Host;

	static_pointer_cast<MeetingClient>(client)->data.chatPermission = true;
	static_pointer_cast<MeetingClient>(client)->data.screenPermission = true;
	static_pointer_cast<MeetingClient>(client)->data.videoPermission = true;
	static_pointer_cast<MeetingClient>(client)->data.voicePermission = true;

	SetClientData(client);
	return client;
}

void MeetingRoom::GetHost(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	Protocol::S_OFFICE_GET_HOST res;
	res.set_clientid(currentHostId);
	client->Send(PacketManager::MakeSendBuffer(res));
}

void MeetingRoom::GetWaitingList(shared_ptr<ClientBase> _client)
{
	if (state != RoomState::Running) return;

	Protocol::S_OFFICE_ADD_WAITING_CLIENT waitingListPkt;
	for (auto wait = waitingClients.begin(); wait != waitingClients.end(); wait++)
	{
		auto client = waitingListPkt.add_clients();
		client->set_clientid(wait->second->clientId);
		client->set_nickname(wait->second->nickname);
	}

	_client->Send(PacketManager::MakeSendBuffer(waitingListPkt));
}

void MeetingRoom::AcceptWait(shared_ptr<ClientBase> _client, string clientId, bool isAccepted)
{
	if (state != RoomState::Running) return;

	if (_client->clientId != currentHostId) return;

	Protocol::S_OFFICE_ACCEPT_WAIT acceptWait;

	//수락하려는 클라이언트가 없는 경우 실패 처리
	auto waitingClient = waitingClients.find(clientId);
	if (waitingClient == waitingClients.end())
	{
		acceptWait.set_success(false);
		//set result as "no client with clientid ~"
		_client->Send(PacketManager::MakeSendBuffer(acceptWait));
		return;
	}

	//인원이 꽉 찬 상태에서 수락하려고 하는 경우 실패 처리
	if (isAccepted && clients.size() >= maxPlayerNumber)
	{
		acceptWait.set_success(false);
		//set result as "room is full"
		_client->Send(PacketManager::MakeSendBuffer(acceptWait));
		return;
	}

	//이외의 경우 성공, 호스트에게 관련 메시지 전송
	acceptWait.set_success(true);
	_client->Send(PacketManager::MakeSendBuffer(acceptWait));

	Protocol::S_OFFICE_REMOVE_WAITING_CLIENT removeWaitingClient;
	auto removedWaitingClient = removeWaitingClient.add_clients();
	removedWaitingClient->set_clientid(waitingClient->second->clientId);
	_client->Send(PacketManager::MakeSendBuffer(removeWaitingClient));

	//대상 클라이언트에게 성공/실패 메시지 전송
	Protocol::S_OFFICE_ACCEPT_WAIT_NOTICE acceptWaitNotice;
	acceptWaitNotice.set_isaccepted(isAccepted);
	waitingClient->second->session->Send(PacketManager::MakeSendBuffer(acceptWaitNotice));

	//입장 허락이었을 경우의 처리, enter 시의 처리와 동일
	if (isAccepted)
	{
		waitingClient->second->data.type = MeetingRoomUserType::Guest;
		waitingClient->second->data.chatPermission = true;
		waitingClient->second->data.videoPermission = false;
		waitingClient->second->data.voicePermission = false;
		waitingClient->second->data.screenPermission = false;

		clients.insert({ waitingClient->second->clientId, waitingClient->second });
		roomInfo["currentPersonnel"] = clients.size();

		Protocol::S_ENTER res;
		res.set_result("SUCCESS");
		waitingClient->second->Send(PacketManager::MakeSendBuffer(res));

		Protocol::S_ADD_CLIENT addClient;
		auto clientInfo = addClient.add_clientinfos();
		clientInfo->set_clientid(waitingClient->second->clientId);
		clientInfo->set_nickname(waitingClient->second->clientId);
		clientInfo->set_statemessage(waitingClient->second->stateMessage);
		Broadcast(PacketManager::MakeSendBuffer(addClient));

		waitingClients.erase(waitingClient);
	}
	else
	{
		waitingClient->second->DoAsync(&ClientBase::Leave, string("WAITING_REJECTED"));
	}
}

void MeetingRoom::Break(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	if (client->clientId != currentHostId) return;

	Protocol::S_OFFICE_BREAK res;
	res.set_success(true);
	client->Send(PacketManager::MakeSendBuffer(res));

	Close();
}

void MeetingRoom::Kick(shared_ptr<ClientBase> client, string clientId)
{
	if (state != RoomState::Running) return;

	if (client->clientId != currentHostId) return;

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

void MeetingRoom::GetPermission(shared_ptr<ClientBase> _client, string clientId)
{
	if (state != RoomState::Running) return;

	Protocol::S_OFFICE_GET_PERMISSION getPermission;

	auto client = clients.find(clientId);
	if (client == clients.end())
		return;

	auto permission = getPermission.mutable_permission();

	auto oClient = static_pointer_cast<MeetingClient>(client->second);

	permission->set_clientid(oClient->clientId);
	permission->set_screenpermission(oClient->data.screenPermission);
	permission->set_chatpermission(oClient->data.chatPermission);
	permission->set_voicepermission(oClient->data.voicePermission);
	permission->set_videopermission(oClient->data.videoPermission);
	permission->set_authority(static_cast<int>(oClient->data.type));

	auto sendBuffer = PacketManager::MakeSendBuffer(getPermission);
	_client->Send(sendBuffer);
}

void MeetingRoom::GetPermissionAll(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	Protocol::S_OFFICE_GET_PERMISSION_ALL getPermission;

	for (auto client = clients.begin(); client != clients.end(); client++)
	{
		auto permission = getPermission.add_permissions();

		auto oClient = static_pointer_cast<MeetingClient>(client->second);

		permission->set_clientid(oClient->clientId);
		permission->set_screenpermission(oClient->data.screenPermission);
		permission->set_chatpermission(oClient->data.chatPermission);
		permission->set_voicepermission(oClient->data.voicePermission);
		permission->set_videopermission(oClient->data.videoPermission);
		permission->set_authority(static_cast<int>(oClient->data.type));
	}

	if (getPermission.permissions_size() > 0)
	{
		auto sendBuffer = PacketManager::MakeSendBuffer(getPermission);
		client->Send(sendBuffer);
	}
}

void MeetingRoom::SetPermission(shared_ptr<ClientBase> _client, Protocol::C_OFFICE_SET_PERMISSION pkt)
{
	if (state != RoomState::Running) return;

	if (_client->clientId != currentHostId) return;

	bool result = true;

	map<string, MeetingRoomUserData> newUserData;
	for (int i = 0; i < pkt.permissions_size(); i++)
	{
		if (clients.find(pkt.permissions()[i].clientid()) == clients.end())
		{
			result = false;
			goto SET_PERMISSION_LOGIC;
		}

		MeetingRoomUserData userData;
		userData.type = static_cast<MeetingRoomUserType>(pkt.permissions()[i].authority());
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

		MeetingRoomUserData userData;
		userData.type = static_pointer_cast<MeetingClient>(client)->data.type;
		userData.screenPermission = static_pointer_cast<MeetingClient>(client)->data.screenPermission;
		userData.chatPermission = static_pointer_cast<MeetingClient>(client)->data.chatPermission;
		userData.videoPermission = static_pointer_cast<MeetingClient>(client)->data.videoPermission;
		userData.voicePermission = static_pointer_cast<MeetingClient>(client)->data.voicePermission;
		newUserData.insert({ client->clientId, userData });
	}

	// 요청 검증
	// 호스트가 하나인지, screen permission 이 하나인지 확인
	// 호스트의 접속 여부는 위에서 확인됨
	{
		int hostCount = 0;
		int screenCount = 0;

		for (auto& [clientId, userData] : newUserData)
		{
			if (userData.type == MeetingRoomUserType::Host)
			{
				hostCount++;
				if (hostCount > 1)
				{
					result = false;
					goto SET_PERMISSION_LOGIC;
				}
			}

			if (userData.screenPermission)
			{
				screenCount++;
				if (screenCount > 1)
				{
					result = false;
					goto SET_PERMISSION_LOGIC;
				}
			}
		}

		if (hostCount == 0)
		{
			result = false;
			goto SET_PERMISSION_LOGIC;
		}
	}

SET_PERMISSION_LOGIC:

	Protocol::S_OFFICE_SET_PERMISSION res;
	res.set_success(result);
	_client->Send(PacketManager::MakeSendBuffer(res));

	if (!result)
		return;
	
	for (auto& [clientId, userData] : newUserData)
	{
		auto client = static_pointer_cast<MeetingClient>(clients.find(clientId)->second);
		if (client->data == userData) continue;

		client->data = userData;

		Protocol::S_OFFICE_GET_PERMISSION permissionNotice;

		auto permission = permissionNotice.mutable_permission();

		permission->set_screenpermission(client->data.screenPermission);
		permission->set_chatpermission(client->data.chatPermission);
		permission->set_voicepermission(client->data.voicePermission);
		permission->set_videopermission(client->data.videoPermission);
		permission->set_authority(static_cast<int>(client->data.type));

		client->Send(PacketManager::MakeSendBuffer(permissionNotice));

		if (client->data.type == MeetingRoomUserType::Host)
		{
			currentHostId = clientId;
			roomInfo["hostName"] = client->nickname;
		}
	}
}

void MeetingRoom::GetRoomInfo(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	Protocol::S_OFFICE_GET_ROOM_INFO roomInfo;

	roomInfo.set_roomname(roomName);
	roomInfo.set_description(description);
	roomInfo.set_topictype(topicType);
	roomInfo.set_password(password);
	roomInfo.set_spaceinfoid(spaceInfoId);
	roomInfo.set_personnel(maxPlayerNumber);
	roomInfo.set_currentpersonnel(clients.size());
	roomInfo.set_observer(0);
	roomInfo.set_currentobserver(0);
	roomInfo.set_currentwaiting(0);
	roomInfo.set_isadvertising(isAdvertising);
	roomInfo.set_thumbnail(thumbnail);
	roomInfo.set_iswaitingroom(isWaitingRoom);
	roomInfo.set_isshutdown(isShutdown);
	roomInfo.set_starttime(createdTimeString);
	roomInfo.set_runningtime(runningTime);
	roomInfo.set_passedtime(passedTime);
	roomInfo.set_roomcode(roomId);

	auto host = clients.find(currentHostId);
	if (host != clients.end())
		roomInfo.set_hostnickname(host->second->nickname);

	client->Send(PacketManager::MakeSendBuffer(roomInfo));
}

void MeetingRoom::SetRoomInfo(shared_ptr<ClientBase> client, Protocol::C_OFFICE_SET_ROOM_INFO pkt)
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

	roomInfo["personnel"] = maxPlayerNumber;
	roomInfo["isPassword"] = isPassword;
	roomInfo["isAdvertising"] = isAdvertising;
	roomInfo["isShutdown"] = isShutdown;

	Protocol::S_OFFICE_SET_ROOM_INFO roomInfo;
	roomInfo.set_success(true);
	client->Send(PacketManager::MakeSendBuffer(roomInfo));
}

void MeetingRoom::HandleVideoStream(string url, float volume, float time, bool play, bool seek, int mediaplayerstate)
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
	this->DoAsync(&MeetingRoom::Broadcast, sendBuffer);
}

void MeetingRoom::HandleShare(shared_ptr<ClientBase> session, bool isShared, int userId)
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

void MeetingRoom::Countdown()
{
	if (state != RoomState::Running) return;

	if (passedTime - runningTime >= 0)
	{
		Close();
		return;
	}

	passedTime++;

	this->DoTimer(60000, &MeetingRoom::Countdown);
}