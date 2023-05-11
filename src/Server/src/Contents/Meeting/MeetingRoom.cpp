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
	for (auto client = waitingList.begin(); client != waitingList.end(); client++)
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
		auto client = waitingList.find(_client->clientId);
		if (client != waitingList.end())
		{
			Protocol::S_OFFICE_REMOVE_WAITING_CLIENT removeWaitingClient;
			removeWaitingClient.add_clients(client->second->clientId);
			clients.find(currentHostId)->second->session->Send(PacketManager::MakeSendBuffer(removeWaitingClient));

			waitingList.erase(client);
			return;
		}
	}

	roomInfo["currentPersonnel"] = clients.size();

	GameRoom::Leave(_client);

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
		client->type = MeetingRoomUserType::Host;
		client->chatPermission = true;
		client->videoPermission = true;
		client->voicePermission = true;
		client->screenPermission = true;
	}
	else if (isWaitingRoom)
	{
		client->type = MeetingRoomUserType::Waiting;
		client->chatPermission = false;
		client->videoPermission = false;
		client->voicePermission = false;
		client->screenPermission = false;
	}
	else
	{
		client->type = MeetingRoomUserType::Guest;
		client->chatPermission = true;
		client->videoPermission = false;
		client->voicePermission = false;
		client->screenPermission = false;
	}
}

void MeetingRoom::OnEnterSuccess(shared_ptr<ClientBase> _client)
{
	auto client = static_pointer_cast<MeetingClient>(_client);

	switch (client->type)
	{
	case MeetingRoomUserType::Host:
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

		SetHost(client->clientId);

		GRoomManager->IndexRoom(static_pointer_cast<RoomBase>(shared_from_this()));

		break;
	}
	case MeetingRoomUserType::Guest:
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

		break;
	}
	case MeetingRoomUserType::Waiting:
	{
		waitingList.insert({ client->clientId, client });

		Protocol::S_ENTER res;
		res.set_result("WAITING");
		client->Send(PacketManager::MakeSendBuffer(res));

		Protocol::S_OFFICE_ADD_WAITING_CLIENT addWaitingClient;
		auto waitingClient = addWaitingClient.add_clients();
		waitingClient->set_clientid(client->clientId);
		waitingClient->set_nickname(client->nickname);
		clients.find(currentHostId)->second->session->Send(PacketManager::MakeSendBuffer(addWaitingClient));

		break;
	}
	default:
		break;
	}
}

shared_ptr<ClientBase> MeetingRoom::MakeClient(string clientId, int sessionId)
{
	auto client = GClientManager->MakeCilent<MeetingClient>(clientId, sessionId, static_pointer_cast<RoomBase>(shared_from_this()));

	static_pointer_cast<MeetingClient>(client)->type = MeetingRoomUserType::Host;

	static_pointer_cast<MeetingClient>(client)->chatPermission = true;
	static_pointer_cast<MeetingClient>(client)->screenPermission = true;
	static_pointer_cast<MeetingClient>(client)->videoPermission = true;
	static_pointer_cast<MeetingClient>(client)->voicePermission = true;

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

string GetCurrentTimeString()
{
	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm bt;

	t += 32400;
	
#ifdef linux
	localtime_r(&t, &bt);
#elif _WIN32
	localtime_s(&bt, &t);
#endif
	char buffer[50];
	strftime(buffer, 50, "%Y.%m.%d %p %I:%M", &bt);
	return string(buffer);
}

void MeetingRoom::SetHost(string clientId)
{
	if (state != RoomState::Running) return;

	if (currentHostId.empty())
	{
		auto host = clients.find(clientId);
		if (host == clients.end())
			return;
		
		currentHostId = clientId;
		roomInfo["hostName"] = host->second->nickname;

		createdTimeString = GetCurrentTimeString();
		roomInfo["createdTime"] = GetCurrentTimeString();

		DoAsync(&MeetingRoom::Countdown);
	}
	else
	{
		if (currentHostId == clientId)
			return;

		auto nextHost = clients.find(clientId);
		if (nextHost == clients.end())
			return;

		auto currentHost = clients.find(currentHostId);

		static_pointer_cast<MeetingClient>(currentHost->second)->type = MeetingRoomUserType::Guest;
		static_pointer_cast<MeetingClient>(currentHost->second)->type = MeetingRoomUserType::Host;

		//기본 권한 세팅은 어떻게?

		currentHostId = clientId;
		roomInfo["hostName"] = nextHost->second->nickname;
	}

	//호스트 이전 알림
	//Protocol::S_MATCHING_HOST hostPkt;
	//hostPkt.set_clientid(currentHostId);
	//Broadcast(PacketManager::MakeSendBuffer(hostPkt));
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
	//if (state != RoomState::Running) return;

	//if (_client->clientId != currentHostId) return;

	//Protocol::S_OFFICE_GET_PERMISSION getPermission;

	//if (clientId.empty())
	//{
	//	for (auto client = clients.begin(); client != clients.end(); client++)
	//	{
	//		auto permission = getPermission.add_permissions();

	//		auto oClient = static_pointer_cast<MeetingClient>(client->second);

	//		permission->set_clientid(oClient->clientId);
	//		permission->set_screenpermission(oClient->screenPermission);
	//		permission->set_chatpermission(oClient->chatPermission);
	//		permission->set_voicepermission(oClient->voicePermission);
	//		permission->set_videopermission(oClient->videoPermission);
	//		permission->set_type(oClient->type);
	//	}
	//}
	//else
	//{
	//	auto client = clients.find(clientId);
	//	if (client != clients.end())
	//	{
	//		auto permission = getPermission.add_permissions();
	//		
	//		auto oClient = static_pointer_cast<MeetingClient>(client->second);

	//		permission->set_clientid(oClient->clientId);
	//		permission->set_screenpermission(oClient->screenPermission);
	//		permission->set_chatpermission(oClient->chatPermission);
	//		permission->set_voicepermission(oClient->voicePermission);
	//		permission->set_videopermission(oClient->videoPermission);
	//		permission->set_type(oClient->type);
	//	}
	//}

	//if (getPermission.permissions_size() > 0)
	//{
	//	auto sendBuffer = PacketManager::MakeSendBuffer(getPermission);
	//	_client->Send(sendBuffer);
	//}
}

void MeetingRoom::SetPermission(shared_ptr<ClientBase> _client, Protocol::C_OFFICE_SET_PERMISSION pkt)
{
	//if (state != RoomState::Running) return;

	//if (_client->clientId != currentHostId) return;

	////예외 상황에 대비해야 함
	////예를 들어, 새로운 호스트를 설정하려고 했는데 그 호스트가 없던 상황이었던 경우 등
	////이러한 경우 해당 작업을 수행하기 전과 동일한 상태를 유지해야 함
	////따라서 로직 시작 전에 먼저 현재 상태를 백업, 예외 상황 발생 시 이전 상태로 되돌려야 함
	////지금은 그러한 작업 없이 받은 대로 상태를 변경시킬 것

	////screen permission 이 하나가 되도록 할 것

	//{
	//	for (int i = 0; i < pkt.permissions_size(); i++)
	//	{
	//		auto permission = pkt.permissions().Get(i);
	//		auto client = clients.find(permission.clientid());
	//		if (client == clients.end())
	//			continue;

	//		auto oClient = static_pointer_cast<MeetingClient>(client->second);

	//		oClient->screenPermission = permission.screenpermission();
	//		oClient->chatPermission = permission.chatpermission();
	//		oClient->voicePermission = permission.voicepermission();
	//		oClient->videoPermission = permission.videopermission();
	//		oClient->type = static_cast<MeetingRoomUserType>(permission.type());

	//		if (oClient->type == MeetingRoomUserType::Host)
	//			currentHostId = client->second->clientId;
	//	}
	//}

	////문제가 없었을 경우

	//{
	//	Protocol::S_OFFICE_SET_PERMISSION permission;
	//	permission.set_success(true);
	//	auto sendBuffer = PacketManager::MakeSendBuffer(permission);
	//	_client->Send(sendBuffer);
	//}

	//{
	//	for (int i = 0; i < pkt.permissions_size(); i++)
	//	{
	//		auto client = clients.find(pkt.permissions().Get(i).clientid());
	//		if (client == clients.end())
	//			continue;

	//		auto oClient = static_pointer_cast<MeetingClient>(client->second);

	//		Protocol::S_OFFICE_SET_PERMISSION_NOTICE permissionNotice;

	//		permissionNotice.set_screenpermission(oClient->screenPermission);
	//		permissionNotice.set_chatpermission(oClient->chatPermission);
	//		permissionNotice.set_voicepermission(oClient->voicePermission);
	//		permissionNotice.set_videopermission(oClient->videoPermission);
	//		permissionNotice.set_type(oClient->type);

	//		auto sendBuffer = PacketManager::MakeSendBuffer(permissionNotice);
	//		client->second->Send(sendBuffer);
	//	}
	//}
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

void MeetingRoom::GetWaitingList(shared_ptr<ClientBase> _client)
{
	if (state != RoomState::Running) return;

	Protocol::S_OFFICE_ADD_WAITING_CLIENT waitingListPkt;
	for (auto wait = waitingList.begin(); wait != waitingList.end(); wait++)
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
	auto waitingClient = waitingList.find(clientId);
	if (waitingClient == waitingList.end())
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
	removeWaitingClient.add_clients(waitingClient->second->clientId);
	_client->Send(PacketManager::MakeSendBuffer(removeWaitingClient));

	//대상 클라이언트에게 성공/실패 메시지 전송
	Protocol::S_OFFICE_ACCEPT_WAIT_NOTICE acceptWaitNotice;
	acceptWaitNotice.set_isaccepted(isAccepted);
	waitingClient->second->session->Send(PacketManager::MakeSendBuffer(acceptWaitNotice));

	//입장 허락이었을 경우의 처리, enter 시의 처리와 동일
	if (isAccepted)
	{
		waitingClient->second->type = MeetingRoomUserType::Guest;
		waitingClient->second->chatPermission = true;
		waitingClient->second->videoPermission = false;
		waitingClient->second->voicePermission = false;
		waitingClient->second->screenPermission = false;

		OnEnterSuccess(waitingClient->second);

		waitingList.erase(waitingClient);
	}
	else
	{
		waitingClient->second->DoAsync(&ClientBase::Leave, string("WAITING_REJECTED"));
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