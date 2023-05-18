#include "LectureRoom.h"

#include "LectureClient.h"
#include "../../Session/GameSession.h"
#include "../../PacketManager.h"

#include "../RoomManager.h"

#include "../ClientManager.h"

LectureRoom::LectureRoom()
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

void LectureRoom::HandleClose()
{
	GameRoom::HandleClose();
}

void LectureRoom::Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) { DoAsync(&LectureRoom::Enter, session, pkt); }

void LectureRoom::Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<ClientBase>& client, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt) { DoAsync(&LectureRoom::InstantiateObject, client, pkt); }
void LectureRoom::Handle_C_INTERACTION_SET_ITEM(shared_ptr<ClientBase>& client, Protocol::C_INTERACTION_SET_ITEM& pkt) {}

void LectureRoom::Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_WAITING_LIST& pkt) { DoAsync(&LectureRoom::GetWaitingList, client); }
void LectureRoom::Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_ACCEPT_WAIT& pkt) { DoAsync(&LectureRoom::AcceptWait, client, pkt.clientid(), pkt.isaccepted()); } 
void LectureRoom::Handle_C_OFFICE_KICK(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_KICK& pkt) { DoAsync(&LectureRoom::Kick, client, pkt.clientid()); }
void LectureRoom::Handle_C_OFFICE_GET_PERMISSION(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_PERMISSION& pkt) { DoAsync(&LectureRoom::GetPermission, client, pkt.clientid()); }
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
	else if ((!pkt.isobserver() && clients.size() >= maxPlayerNumber) || (pkt.isobserver() && observers.size() >= maxObserverNumber))
	{
		enterResult = { true, "ROOM_IS_FULL" };
	}

	Protocol::S_ENTER res;
	res.set_result(enterResult.second);

	if (!enterResult.first)
	{
		session->Send(PacketManager::MakeSendBuffer(res));
		return;
	}

	auto client = static_pointer_cast<LectureClient>(
		GClientManager->MakeCilent<LectureClient>(pkt.clientid(), pkt.sessionid(), static_pointer_cast<RoomBase>(shared_from_this()))
	);

	client->session = session;
	session->owner = client;

	SetDefaultClientData(client);

	if (currentHostId.empty() && creatorId == client->clientId)
	{
		client->type = LectureRoomUserType::Host;
		client->chatPermission = true;
		client->videoPermission = true;
		client->voicePermission = true;
		client->screenPermission = true;
	}
	else
	{
		if (pkt.isobserver())
		{
			client->type = LectureRoomUserType::Observer;
			client->chatPermission = false;
			client->videoPermission = false;
			client->voicePermission = false;
			client->screenPermission = false;
		}
		else
		{
			client->type = LectureRoomUserType::Audience;
			client->chatPermission = true;
			client->videoPermission = false;
			client->voicePermission = false;
			client->screenPermission = false;
		}
	}

	if (client->type == LectureRoomUserType::Host)
	{
		clients.insert({ client->clientId, client });
		roomInfo["currentPersonnel"] = clients.size();

		res.set_result("SUCCESS");
		client->Send(PacketManager::MakeSendBuffer(res));

		Protocol::S_ADD_CLIENT addClient;
		auto clientInfo = addClient.add_clientinfos();
		clientInfo->set_clientid(client->clientId);
		clientInfo->set_nickname(client->nickname);
		clientInfo->set_statemessage(client->stateMessage);
		Broadcast(PacketManager::MakeSendBuffer(addClient));

		SetHost(client->clientId);
	}
	else if (isWaitingRoom)
	{
		if (pkt.isobserver())
		{
			waitingObservers.insert({ client->clientId, client });
		}
		else
		{
			waitingClients.insert({ client->clientId, client });
		}

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
			observers.insert({ client->clientId, client });
		}
		else
		{
			clients.insert({ client->clientId, client });
			roomInfo["currentPersonnel"] = clients.size();
		}

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

void LectureRoom::SetHost(string clientId)
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

		DoAsync(&LectureRoom::Countdown);

		GRoomManager->IndexRoom(static_pointer_cast<RoomBase>(shared_from_this()));
	}
	else
	{
		if (currentHostId == clientId)
			return;

		auto nextHost = clients.find(clientId);
		if (nextHost == clients.end())
			return;

		auto currentHost = clients.find(currentHostId);

		static_pointer_cast<LectureClient>(currentHost->second)->type = LectureRoomUserType::Audience;
		static_pointer_cast<LectureClient>(currentHost->second)->chatPermission = true;
		static_pointer_cast<LectureClient>(currentHost->second)->voicePermission = false;
		static_pointer_cast<LectureClient>(currentHost->second)->videoPermission = false;
		static_pointer_cast<LectureClient>(currentHost->second)->screenPermission = false;

		static_pointer_cast<LectureClient>(nextHost->second)->type = LectureRoomUserType::Host;
		static_pointer_cast<LectureClient>(nextHost->second)->chatPermission = true;
		static_pointer_cast<LectureClient>(nextHost->second)->voicePermission = true;
		static_pointer_cast<LectureClient>(nextHost->second)->videoPermission = true;
		//screen permission 변경하지 않음

		currentHostId = clientId;
		roomInfo["hostName"] = nextHost->second->nickname;

		Protocol::S_OFFICE_GET_PERMISSION toCurrent;
		auto currentHostInfo = toCurrent.mutable_permission();
		currentHostInfo->set_clientid(currentHost->second->clientId);
		currentHostInfo->set_authority(static_cast<int>(static_pointer_cast<LectureClient>(currentHost->second)->type));
		currentHostInfo->set_chatpermission(static_pointer_cast<LectureClient>(currentHost->second)->chatPermission);
		currentHostInfo->set_voicepermission(static_pointer_cast<LectureClient>(currentHost->second)->voicePermission);
		currentHostInfo->set_videopermission(static_pointer_cast<LectureClient>(currentHost->second)->videoPermission);
		currentHostInfo->set_screenpermission(static_pointer_cast<LectureClient>(currentHost->second)->screenPermission);
		currentHost->second->Send(PacketManager::MakeSendBuffer(toCurrent));

		Protocol::S_OFFICE_GET_PERMISSION toNext;
		auto nextHostInfo = toNext.mutable_permission();
		nextHostInfo->set_clientid(nextHost->second->clientId);
		nextHostInfo->set_authority(static_cast<int>(static_pointer_cast<LectureClient>(nextHost->second)->type));
		nextHostInfo->set_chatpermission(static_pointer_cast<LectureClient>(nextHost->second)->chatPermission);
		nextHostInfo->set_voicepermission(static_pointer_cast<LectureClient>(nextHost->second)->voicePermission);
		nextHostInfo->set_videopermission(static_pointer_cast<LectureClient>(nextHost->second)->videoPermission);
		nextHostInfo->set_screenpermission(static_pointer_cast<LectureClient>(nextHost->second)->screenPermission);
		nextHost->second->Send(PacketManager::MakeSendBuffer(toNext));
	}
}

void LectureRoom::Leave(shared_ptr<ClientBase> _client)
{
	if (state != RoomState::Running) return;

	auto client = static_pointer_cast<LectureClient>(_client);
	
	//대기열 먼저 확인, 존재했으면 지우고 호스트에게 알림
	{
		auto targetClients = client->type == LectureRoomUserType::Observer ? waitingObservers : waitingClients;

		auto waitingClient = targetClients.find(client->clientId);
		if (waitingClient != waitingClients.end())
		{
			Protocol::S_OFFICE_REMOVE_WAITING_CLIENT removedWaitingClients;
			auto removedWaitingClient = removedWaitingClients.add_clients();
			removedWaitingClient->set_clientid(waitingClient->second->clientId);
			removedWaitingClient->set_isobserver(client->type == LectureRoomUserType::Observer);
			clients.find(currentHostId)->second->session->Send(PacketManager::MakeSendBuffer(removedWaitingClients));

			targetClients.erase(waitingClient);
			return;
		}
	}

	GameRoom::Leave(_client);

	if(client->type != LectureRoomUserType::Observer)
		roomInfo["currentPersonnel"] = clients.size();

	if (_client->clientId == currentHostId)
		Close();
}

void LectureRoom::GetWaitingList(shared_ptr<ClientBase> _client)
{
	//if (state != RoomState::Running) return;

	//Protocol::S_OFFICE_ADD_WAITING_CLIENT waitingListPkt;
	//for (auto wait = waitingList.begin(); wait != waitingList.end(); wait++)
	//{
	//	auto client = waitingListPkt.add_clients();
	//	client->set_isobserver(wait->second.first);
	//	client->set_clientid(wait->second.second->clientId);
	//	client->set_nickname(wait->second.second->nickname);
	//}

	//_client->Send(PacketManager::MakeSendBuffer(waitingListPkt));
}

void LectureRoom::AcceptWait(shared_ptr<ClientBase> _client, string clientId, bool isAccepted)
{
	//if (state != RoomState::Running) return;

	//if (_client->clientId != currentHostId) return;

	//Protocol::S_OFFICE_ACCEPT_WAIT acceptWait;

	//auto client = waitingList.find(clientId);
	//if (client == waitingList.end())
	//{
	//	acceptWait.set_success(false);
	//	//set result as "no client with clientid ~"
	//	_client->Send(PacketManager::MakeSendBuffer(acceptWait));
	//	return;
	//}

	////인원이 꽉 찬 상태에서 수락하려고 하는 경우 실패 처리
	//if (!client->second.first && maxPlayerNumber <= currentPersonnel
	//	|| client->second.first && maxObserverNumber <= currentObserver)
	//{
	//	if (isAccepted)
	//	{
	//		acceptWait.set_success(false);
	//		//set result as "room is full"
	//		_client->Send(PacketManager::MakeSendBuffer(acceptWait));
	//		return;
	//	}
	//}

	////이외의 경우 성공, 호스트에게 관련 메시지 전송
	//acceptWait.set_success(true);
	//_client->Send(PacketManager::MakeSendBuffer(acceptWait));

	//Protocol::S_OFFICE_REMOVE_WAITING_CLIENT removeWaitingClient;
	//removeWaitingClient.add_clients(client->second.second->clientId);
	//_client->Send(PacketManager::MakeSendBuffer(removeWaitingClient));

	////대상 클라이언트에게 성공/실패 메시지 전송
	//Protocol::S_OFFICE_ACCEPT_WAIT_NOTICE acceptWaitNotice;
	//acceptWaitNotice.set_isaccepted(isAccepted);
	//client->second.second->session->Send(PacketManager::MakeSendBuffer(acceptWaitNotice));

	////입장 허락이었을 경우의 처리, enter 시의 처리와 동일
	//if (isAccepted)
	//{
	//	if (client->second.second->type == LectureRoomUserType::Observer)
	//		client->second.second->chatPermission = false;
	//	else
	//		client->second.second->chatPermission = true;

	//	client->second.second->screenPermission = false;
	//	client->second.second->videoPermission = false;
	//	client->second.second->voicePermission = false;

	//	clients.insert({ client->second.second->clientId, client->second.second });

	//	if (client->second.first)
	//		currentObserver++;
	//	else
	//	{
	//		currentPersonnel++;
	//		roomInfo["currentPersonnel"] = currentPersonnel;
	//	}
	//}

	////대기열에서 대상 퇴장
	//client->second.second->DoAsync(&ClientBase::Leave, string("WAITING_REJECTED"));
}

void LectureRoom::Kick(shared_ptr<ClientBase> client, string clientId)
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

void LectureRoom::GetPermission(shared_ptr<ClientBase> _client, string clientId)
{
	if (state != RoomState::Running) return;

	if (_client->clientId != currentHostId) return;

	if (clientId.empty())
	{
		Protocol::S_OFFICE_GET_PERMISSION_ALL getPermissionAll;

		for (auto client = clients.begin(); client != clients.end(); client++)
		{
			auto permission = getPermissionAll.add_permissions();

			auto oClient = static_pointer_cast<LectureClient>(client->second);

			permission->set_clientid(oClient->clientId);
			permission->set_screenpermission(oClient->screenPermission);
			permission->set_chatpermission(oClient->chatPermission);
			permission->set_voicepermission(oClient->voicePermission);
			permission->set_videopermission(oClient->videoPermission);
			permission->set_authority(static_cast<int>(oClient->type));
		}

		if (getPermissionAll.permissions_size() > 0)
		{
			auto sendBuffer = PacketManager::MakeSendBuffer(getPermissionAll);
			_client->Send(sendBuffer);
		}
	}
	else
	{
		auto client = clients.find(clientId);
		if (client != clients.end())
		{
			Protocol::S_OFFICE_GET_PERMISSION getPermission;
			auto permission = getPermission.mutable_permission();
			
			auto oClient = static_pointer_cast<LectureClient>(client->second);

			permission->set_clientid(oClient->clientId);
			permission->set_screenpermission(oClient->screenPermission);
			permission->set_chatpermission(oClient->chatPermission);
			permission->set_voicepermission(oClient->voicePermission);
			permission->set_videopermission(oClient->videoPermission);
			permission->set_authority(static_cast<int>(oClient->type));
		}
	}
}

void LectureRoom::SetPermission(shared_ptr<ClientBase> _client, Protocol::C_OFFICE_SET_PERMISSION pkt)
{
	if (state != RoomState::Running) return;

	if (_client->clientId != currentHostId) return;

	// 요청 검증
	// 호스트가 하나인지, 해당 호스트가 접속해 있는지 확인
	// screen permission 이 하나인지 확인

	bool result = true;
	{
		int hostCount = 0;
		int screenCount = 0;

		for (int i = 0; i < pkt.permissions_size(); i++)
		{
			if (pkt.permissions()[i].authority() == static_cast<int>(LectureRoomUserType::Host))
			{
				if (clients.find(pkt.permissions()[i].clientid()) == clients.end())
				{
					result = false;
					goto SET_PERMISSION_LOGIC;
				}

				hostCount++;
				if (hostCount > 1)
				{
					result = false;
					goto SET_PERMISSION_LOGIC;
				}
			}

			if (pkt.permissions()[i].screenpermission())
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

	//검증을 통과했을 경우
	for (int i = 0; i < pkt.permissions_size(); i++)
	{
		auto permission = pkt.permissions().Get(i);
		auto client = clients.find(permission.clientid());
		if (client == clients.end())
			continue;

		auto oClient = static_pointer_cast<LectureClient>(client->second);

		oClient->screenPermission = permission.screenpermission();
		oClient->chatPermission = permission.chatpermission();
		oClient->voicePermission = permission.voicepermission();
		oClient->videoPermission = permission.videopermission();
		oClient->type = static_cast<LectureRoomUserType>(permission.authority());

		if (oClient->type == LectureRoomUserType::Host)
			currentHostId = client->second->clientId;
	}

	{
		Protocol::S_OFFICE_SET_PERMISSION permission;
		permission.set_success(true);
		auto sendBuffer = PacketManager::MakeSendBuffer(permission);
		_client->Send(sendBuffer);
	}

	{
		for (int i = 0; i < pkt.permissions_size(); i++)
		{
			auto client = clients.find(pkt.permissions().Get(i).clientid());
			if (client == clients.end())
				continue;

			auto oClient = static_pointer_cast<LectureClient>(client->second);

			Protocol::S_OFFICE_GET_PERMISSION permissionNotice;

			auto permission = permissionNotice.mutable_permission();

			permission->set_screenpermission(oClient->screenPermission);
			permission->set_chatpermission(oClient->chatPermission);
			permission->set_voicepermission(oClient->voicePermission);
			permission->set_videopermission(oClient->videoPermission);
			permission->set_authority(static_cast<int>(oClient->type));

			auto sendBuffer = PacketManager::MakeSendBuffer(permissionNotice);
			client->second->Send(sendBuffer);
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

	roomInfo["personnel"] = maxPlayerNumber;
	roomInfo["isPassword"] = isPassword;
	roomInfo["isAdvertising"] = isAdvertising;
	roomInfo["isShutdown"] = isShutdown;

	Protocol::S_OFFICE_SET_ROOM_INFO roomInfo;
	roomInfo.set_success(true);
	client->Send(PacketManager::MakeSendBuffer(roomInfo));
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

	if (oClient->type == LectureRoomUserType::Observer)
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

	if (oClient->type == LectureRoomUserType::Observer)
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

	if (oClient->type == LectureRoomUserType::Observer)
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

void LectureRoom::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	RoomBase::Broadcast(sendBuffer);

	for (const auto& [clientId, observer] : observers)
		observer->Send(sendBuffer);
}