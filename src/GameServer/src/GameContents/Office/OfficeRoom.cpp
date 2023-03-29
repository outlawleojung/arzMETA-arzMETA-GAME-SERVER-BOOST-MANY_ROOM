#include "OfficeRoom.h"

#include "OfficeClient.h"
#include "../../Session/GameSession.h"
#include "../../ClientPacketHandler.h"

#include "../ClientManager.h"

OfficeRoom::OfficeRoom(vector<string> sceneIds)
	: GameRoom(sceneIds)
	, currentPersonnel(0)
	, currentObserver(0)
	, isWaitingRoom(false)
	, isPassword(false)
	, isAdvertising(false)
	, isShutdown(false)
	, passedTime(0)
	, topicType(0)
	, currentHostId("")
{
	disconnectedSessionWaitTime = 10000;
}

void OfficeRoom::Init()
{
	state = RoomState::Running;

	if (AUTO_DESTROY)
	{
		this->DoTimer(30000, std::function<void()>(
			[this]() {
				if (this->state != RoomState::Running)
					return;

				if (this->clients.size() == 0)
					Close();
			}
		));
	}

	this->DoAsync(&OfficeRoom::Countdown);
}

void OfficeRoom::HandleClose()
{
	for (auto client = waitingList.begin(); client != waitingList.end(); client++)
		client->second.second->DoAsync(&ClientBase::Leave, string("Closing"));

	GameRoom::HandleClose();
}

void OfficeRoom::Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) { DoAsync(&OfficeRoom::Enter, session, pkt); }

void OfficeRoom::Handle_C_BASE_SET_SCENE(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_SCENE& pkt) { DoAsync(&OfficeRoom::SetScene, client, pkt.sceneid()); }
void OfficeRoom::Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<ClientBase>& client, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt) { DoAsync(&OfficeRoom::InstantiateObject, client, pkt); }

void OfficeRoom::Handle_C_INTERACTION_SET_ITEM(shared_ptr<ClientBase>& client, Protocol::C_INTERACTION_SET_ITEM& pkt) {}

void OfficeRoom::Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_WAITING_LIST& pkt) { DoAsync(&OfficeRoom::GetWaitingList, client); }
void OfficeRoom::Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_ACCEPT_WAIT& pkt) { DoAsync(&OfficeRoom::AcceptWait, client, pkt.clientid(), pkt.isaccepted()); } 
void OfficeRoom::Handle_C_OFFICE_GET_HOST(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_HOST& pkt) { DoAsync(&OfficeRoom::GetHost, client); }
void OfficeRoom::Handle_C_OFFICE_BREAK(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_BREAK& pkt) { DoAsync(&OfficeRoom::Break, client); }
void OfficeRoom::Handle_C_OFFICE_KICK(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_KICK& pkt) { DoAsync(&OfficeRoom::Kick, client, pkt.clientid()); }
void OfficeRoom::Handle_C_OFFICE_GET_PERMISSION(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_PERMISSION& pkt) { DoAsync(&OfficeRoom::GetPermission, client, pkt.clientid()); }
void OfficeRoom::Handle_C_OFFICE_SET_PERMISSION(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_SET_PERMISSION& pkt) { DoAsync(&OfficeRoom::SetPermission, client, pkt); }
void OfficeRoom::Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_SET_ROOM_INFO& pkt) { DoAsync(&OfficeRoom::SetRoomInfo, client, pkt); }
void OfficeRoom::Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_ROOM_INFO& pkt) { DoAsync(&OfficeRoom::GetRoomInfo, client); }
void OfficeRoom::Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_VIDEO_STREAM& pkt) { DoAsync(&OfficeRoom::HandleVideoStream, pkt.url(), pkt.volume(), pkt.time(), pkt.play(), pkt.seek(), pkt.mediaplayerstate()); }

void OfficeRoom::Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt)
{
	Protocol::S_ENTER res;

	if (state != RoomState::Running)
	{
		session->Disconnect();
		return;
	}

	{
		auto client = waitingList.find(pkt.clientid());
		if (client != waitingList.end())
		{
			client->second.second->DoAsync(&ClientBase::Leave, string("DUPLICATED"));
			DoTimer(1000, &OfficeRoom::Enter, session, pkt);
			return;
		}
	}

	{
		auto client = clients.find(pkt.clientid());
		if (client != clients.end())
		{
			client->second->DoAsync(&ClientBase::Leave, string("DUPLICATED"));
			DoTimer(1000, &OfficeRoom::Enter, session, pkt);
			return;
		}
	}

	GLogManager->Log("Session Try to Enter :		", pkt.clientid());
	
	if (currentHostId.empty() && creatorId == pkt.clientid())
	{
		auto client = static_pointer_cast<OfficeClient>(GClientManager->MakeCilent<OfficeClient>(session, pkt.clientid(), pkt.nickname(), static_pointer_cast<RoomBase>(shared_from_this())));

		client->type = OfficeRoomUserType::Host;

		client->chatPermission = true;
		client->screenPermission = true;
		client->videoPermission = true;
		client->voicePermission = true;

		clients.insert({ pkt.clientid(), client });

		currentPersonnel++;
		currentHostId = pkt.clientid();

		res.set_result("SUCCESS");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));

		return;
	}

	//if (clients.size() == 0)
	//{
	//	res.set_result("HOST_NOT_ENTERED");
	//	session->Send(ClientPacketHandler::MakeSendBuffer(res));
	//	session->Disconnect();
	//	return;
	//}

	if (isShutdown)
	{
		res.set_result("ROOM_IS_SHUTDONW");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		session->Disconnect();
		return;
	}

	if (isPassword && password != pkt.password())
	{
		res.set_result("PASSWORD_FAIL");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		session->Disconnect();
		return;
	}

	if (isWaitingRoom)
	{		
		auto client = static_pointer_cast<OfficeClient>(GClientManager->MakeCilent<OfficeClient>(session, pkt.clientid(), pkt.nickname(), static_pointer_cast<RoomBase>(shared_from_this())));

		if (pkt.isobserver())
			client->type = OfficeRoomUserType::Observer;
		else
			if (modeType == 1)
				client->type = OfficeRoomUserType::Guest;
			else if (modeType == 2)
				client->type = OfficeRoomUserType::Audience;

		client->chatPermission = false;
		client->screenPermission = false;
		client->videoPermission = false;
		client->voicePermission = false;

		waitingList.insert({ pkt.clientid(), { pkt.isobserver(),client } });

		res.set_result("WAITING");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));

		Protocol::S_OFFICE_ADD_WAITING_CLIENT addWaitingClient;
		auto waitingClient = addWaitingClient.add_clients();
		waitingClient->set_isobserver(pkt.isobserver());
		waitingClient->set_clientid(pkt.clientid());
		waitingClient->set_nickname(pkt.nickname());
		clients.find(currentHostId)->second->session->Send(ClientPacketHandler::MakeSendBuffer(addWaitingClient));

		return;	
	}

	if ((!pkt.isobserver() && (maxPlayerNumber <= currentPersonnel)) || (pkt.isobserver() && (observer <= currentObserver)))
	{
		res.set_result("ROOM_IS_FULL");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		session->Disconnect();
		return;
	}

	auto client = static_pointer_cast<OfficeClient>(GClientManager->MakeCilent<OfficeClient>(session, pkt.clientid(), pkt.nickname(), static_pointer_cast<RoomBase>(shared_from_this())));

	if (pkt.isobserver())
	{
		client->type = OfficeRoomUserType::Observer;
		client->chatPermission = false;
	}
	else
	{
		if (modeType == 1)
			client->type = OfficeRoomUserType::Guest;
		else if (modeType == 2)
			client->type = OfficeRoomUserType::Audience;

		client->chatPermission = true;
	}

	client->screenPermission = false;
	client->videoPermission = false;
	client->voicePermission = false;

	clients.insert({ pkt.clientid(), client });

	res.set_result("SUCCESS");
	session->Send(ClientPacketHandler::MakeSendBuffer(res));

	if (pkt.isobserver())
		currentObserver++;
	else
		currentPersonnel++;
}

void OfficeRoom::Leave(shared_ptr<ClientBase> _client)
{
	auto oClient = static_pointer_cast<OfficeClient>(_client);

	//대기열 먼저 확인, 존재했으면 지우고 호스트에게 알림
	{
		auto client = waitingList.find(_client->clientId);
		if (client != waitingList.end())
		{
			Protocol::S_OFFICE_REMOVE_WAITING_CLIENT removeWaitingClient;
			removeWaitingClient.add_clients(client->second.second->clientId);
			clients.find(currentHostId)->second->session->Send(ClientPacketHandler::MakeSendBuffer(removeWaitingClient));

			waitingList.erase(client);
			return;
		}
	}
	
	//인원수 조정
	if (oClient->type == OfficeRoomUserType::Observer)
		currentObserver--;
	else
		currentPersonnel--;

	GameRoom::Leave(_client);

	if (DESTROY_WHEN_EMPTY)
		if (clients.size() == 0 || _client->clientId == currentHostId)
			Close();
}

void OfficeRoom::GetHost(shared_ptr<ClientBase> client)
{
	Protocol::S_OFFICE_GET_HOST res;
	res.set_clientid(currentHostId);
	client->Send(ClientPacketHandler::MakeSendBuffer(res));
}

void OfficeRoom::Break(shared_ptr<ClientBase> client)
{
	if (client->clientId != currentHostId) return;

	Protocol::S_OFFICE_BREAK res;
	res.set_success(true);
	client->Send(ClientPacketHandler::MakeSendBuffer(res));

	Close();
}

void OfficeRoom::Kick(shared_ptr<ClientBase> client, string clientId)
{
	if (client->clientId != currentHostId) return;

	Protocol::S_OFFICE_KICK res;

	//존재하는지 확인
	auto kickedClient = clients.find(clientId);
	if (kickedClient == clients.end())
	{
		res.set_success(false);
		client->Send(ClientPacketHandler::MakeSendBuffer(res));
		return;
	}

	kickedClient->second->DoAsync(&ClientBase::Leave, string("Kicked"));

	res.set_success(true);
	client->Send(ClientPacketHandler::MakeSendBuffer(res));
}

void OfficeRoom::GetPermission(shared_ptr<ClientBase> _client, string clientId)
{
	if (_client->clientId != currentHostId) return;

	Protocol::S_OFFICE_GET_PERMISSION getPermission;

	if (clientId.empty())
	{
		for (auto client = clients.begin(); client != clients.end(); client++)
		{
			auto permission = getPermission.add_permissions();

			auto oClient = static_pointer_cast<OfficeClient>(client->second);

			permission->set_clientid(oClient->clientId);
			permission->set_screenpermission(oClient->screenPermission);
			permission->set_chatpermission(oClient->chatPermission);
			permission->set_voicepermission(oClient->voicePermission);
			permission->set_videopermission(oClient->videoPermission);
			permission->set_type(oClient->type);
		}
	}
	else
	{
		auto client = clients.find(clientId);
		if (client != clients.end())
		{
			auto permission = getPermission.add_permissions();
			
			auto oClient = static_pointer_cast<OfficeClient>(client->second);

			permission->set_clientid(oClient->clientId);
			permission->set_screenpermission(oClient->screenPermission);
			permission->set_chatpermission(oClient->chatPermission);
			permission->set_voicepermission(oClient->voicePermission);
			permission->set_videopermission(oClient->videoPermission);
			permission->set_type(oClient->type);
		}
	}

	if (getPermission.permissions_size() > 0)
	{
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(getPermission);
		_client->Send(sendBuffer);
	}
}

void OfficeRoom::SetPermission(shared_ptr<ClientBase> _client, Protocol::C_OFFICE_SET_PERMISSION pkt)
{
	if (_client->clientId != currentHostId) return;

	//예외 상황에 대비해야 함
	//예를 들어, 새로운 호스트를 설정하려고 했는데 그 호스트가 없던 상황이었던 경우 등
	//이러한 경우 해당 작업을 수행하기 전과 동일한 상태를 유지해야 함
	//따라서 로직 시작 전에 먼저 현재 상태를 백업, 예외 상황 발생 시 이전 상태로 되돌려야 함
	//지금은 그러한 작업 없이 받은 대로 상태를 변경시킬 것

	//screen permission 이 하나가 되도록 할 것

	{
		for (int i = 0; i < pkt.permissions_size(); i++)
		{
			auto permission = pkt.permissions().Get(i);
			auto client = clients.find(permission.clientid());
			if (client == clients.end())
				continue;

			auto oClient = static_pointer_cast<OfficeClient>(client->second);

			oClient->screenPermission = permission.screenpermission();
			oClient->chatPermission = permission.chatpermission();
			oClient->voicePermission = permission.voicepermission();
			oClient->videoPermission = permission.videopermission();
			oClient->type = static_cast<OfficeRoomUserType>(permission.type());

			if (oClient->type == OfficeRoomUserType::Host)
				currentHostId = client->second->clientId;
		}
	}

	//문제가 없었을 경우

	{
		Protocol::S_OFFICE_SET_PERMISSION permission;
		permission.set_success(true);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(permission);
		_client->Send(sendBuffer);
	}

	{
		for (int i = 0; i < pkt.permissions_size(); i++)
		{
			auto client = clients.find(pkt.permissions().Get(i).clientid());
			if (client == clients.end())
				continue;

			auto oClient = static_pointer_cast<OfficeClient>(client->second);

			Protocol::S_OFFICE_SET_PERMISSION_NOTICE permissionNotice;

			permissionNotice.set_screenpermission(oClient->screenPermission);
			permissionNotice.set_chatpermission(oClient->chatPermission);
			permissionNotice.set_voicepermission(oClient->voicePermission);
			permissionNotice.set_videopermission(oClient->videoPermission);
			permissionNotice.set_type(oClient->type);

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(permissionNotice);
			client->second->Send(sendBuffer);
		}
	}
}

void OfficeRoom::GetRoomInfo(shared_ptr<ClientBase> client)
{
	Protocol::S_OFFICE_GET_ROOM_INFO roomInfo;

	roomInfo.set_roomname(roomName);
	roomInfo.set_description(description);
	roomInfo.set_topictype(topicType);
	roomInfo.set_password(password);
	roomInfo.set_spaceinfoid(spaceInfoId);
	roomInfo.set_personnel(maxPlayerNumber);
	roomInfo.set_currentpersonnel(currentPersonnel);
	roomInfo.set_isadvertising(isAdvertising);
	roomInfo.set_thumbnail(thumbnail);
	roomInfo.set_iswaitingroom(isWaitingRoom);
	roomInfo.set_isshutdown(isShutdown);
	roomInfo.set_runningtime(runningTime);
	roomInfo.set_passedtime(passedTime);
	roomInfo.set_roomcode(roomId);
		
	auto host = clients.find(currentHostId);
	if (host != clients.end())
		roomInfo.set_hostnickname(host->second->nickname);

	client->Send(ClientPacketHandler::MakeSendBuffer(roomInfo));
}

void OfficeRoom::SetRoomInfo(shared_ptr<ClientBase> client, Protocol::C_OFFICE_SET_ROOM_INFO pkt)
{
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

	Protocol::S_OFFICE_SET_ROOM_INFO roomInfo;
	roomInfo.set_success(true);
	client->Send(ClientPacketHandler::MakeSendBuffer(roomInfo));
}

void OfficeRoom::HandleVideoStream(string url, float volume, float time, bool play, bool seek, int mediaplayerstate)
{
	Protocol::S_OFFICE_VIDEO_STREAM videoStream;

	videoStream.set_url(url);
	videoStream.set_volume(volume);
	videoStream.set_time(time);
	videoStream.set_play(play);
	videoStream.set_seek(seek);
	videoStream.set_mediaplayerstate(mediaplayerstate);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(videoStream);
	this->DoAsync(&OfficeRoom::Broadcast, sendBuffer);
}

void OfficeRoom::GetWaitingList(shared_ptr<ClientBase> _client)
{
	Protocol::S_OFFICE_ADD_WAITING_CLIENT waitingListPkt;
	for (auto wait = waitingList.begin(); wait != waitingList.end(); wait++)
	{
		auto client = waitingListPkt.add_clients();
		client->set_isobserver(wait->second.first);
		client->set_clientid(wait->second.second->clientId);
		client->set_nickname(wait->second.second->nickname);
	}

	_client->Send(ClientPacketHandler::MakeSendBuffer(waitingListPkt));
}

void OfficeRoom::AcceptWait(shared_ptr<ClientBase> _client, string clientId, bool isAccepted)
{
	if (_client->clientId != currentHostId) return;

	Protocol::S_OFFICE_ACCEPT_WAIT acceptWait;

	auto client = waitingList.find(clientId);
	if (client == waitingList.end())
	{
		acceptWait.set_success(false);
		//set result as "no client with clientid ~"
		_client->Send(ClientPacketHandler::MakeSendBuffer(acceptWait));
		return;
	}
	
	//인원이 꽉 찬 상태에서 수락하려고 하는 경우 실패 처리
	if ( !client->second.first && maxPlayerNumber <= currentPersonnel
		|| client->second.first && observer <= currentObserver)
	{
		if (isAccepted)
		{
			acceptWait.set_success(false);
			//set result as "room is full"
			_client->Send(ClientPacketHandler::MakeSendBuffer(acceptWait));
			return;
		}
	}

	//이외의 경우 성공, 호스트에게 관련 메시지 전송
	acceptWait.set_success(true);
	_client->Send(ClientPacketHandler::MakeSendBuffer(acceptWait));

	Protocol::S_OFFICE_REMOVE_WAITING_CLIENT removeWaitingClient;
	removeWaitingClient.add_clients(client->second.second->clientId);
	_client->Send(ClientPacketHandler::MakeSendBuffer(removeWaitingClient));

	//대상 클라이언트에게 성공/실패 메시지 전송
	Protocol::S_OFFICE_ACCEPT_WAIT_NOTICE acceptWaitNotice;
	acceptWaitNotice.set_isaccepted(isAccepted);
	client->second.second->session->Send(ClientPacketHandler::MakeSendBuffer(acceptWaitNotice));

	//입장 허락이었을 경우의 처리, enter 시의 처리와 동일
	if (isAccepted)
	{
		if (client->second.second->type == OfficeRoomUserType::Observer)
			client->second.second->chatPermission = false;
		else
			client->second.second->chatPermission = true;

		client->second.second->screenPermission = false;
		client->second.second->videoPermission = false;
		client->second.second->voicePermission = false;

		clients.insert({ client->second.second->clientId, client->second.second });

		if (client->second.first)
			currentObserver++;
		else
			currentPersonnel++;
	}

	//대기열에서 대상 퇴장
	client->second.second->DoAsync(&ClientBase::Leave, string("Waiting_Rejected"));
}

void OfficeRoom::SetScene(shared_ptr<ClientBase> _client, string sceneId)
{
	auto client = clients.find(_client->clientId);
	if (client == clients.end())
		return;

	GameRoom::SetScene(_client, sceneId);
}

void OfficeRoom::InstantiateObject(shared_ptr<ClientBase> _client, Protocol::C_BASE_INSTANTIATE_OBJECT pkt)
{
	auto client = clients.find(_client->clientId);
	if (client == clients.end())
		return;

	auto oClient = static_pointer_cast<OfficeClient>(client->second);

	if (oClient->type == OfficeRoomUserType::Observer)
		return;

	GameRoom::Handle_C_BASE_INSTANTIATE_OBJECT(_client, pkt);
}

void OfficeRoom::SetState(shared_ptr<ClientBase> _client, Protocol::C_INTERACTION_SET_ITEM pkt)
{
	auto client = clients.find(_client->clientId);
	if (client == clients.end())
		return;

	auto oClient = static_pointer_cast<OfficeClient>(client->second);

	if (oClient->type == OfficeRoomUserType::Observer)
		return;

	GameRoom::Handle_C_INTERACTION_SET_ITEM(_client, pkt);
}

void OfficeRoom::RemoveState(shared_ptr<ClientBase> _client, Protocol::C_INTERACTION_REMOVE_ITEM pkt)
{
	auto client = clients.find(_client->clientId);
	if (client == clients.end())
		return;

	auto oClient = static_pointer_cast<OfficeClient>(client->second);

	if (oClient->type == OfficeRoomUserType::Observer)
		return;

	GameRoom::Handle_C_INTERACTION_REMOVE_ITEM(_client, pkt);
}

void OfficeRoom::Countdown()
{
	if (state != RoomState::Running)
		return;

	if (passedTime - runningTime >= 0)
	{
		Close();
		return;
	}

	passedTime++;

	this->DoTimer(60000, &OfficeRoom::Countdown);
}


nlohmann::json OfficeRoom::ToJson()
{
	nlohmann::json json;

	json["roomId"] = roomId;

	json["roomName"] = roomName;
	json["description"] = description;
	json["spaceInfoId"] = spaceInfoId;
	json["modeType"] = modeType;
	json["topicType"] = topicType;
	json["thumbnail"] = thumbnail;
	
	auto host = clients.find(currentHostId);
	if (host != clients.end())
		json["host"] = host->second->nickname;
	else
		json["host"] = "";

	json["personnel"] = maxPlayerNumber;
	json["currentPersonnel"] = currentPersonnel;
	json["isPassword"] = isPassword;
	json["isAdvertising"] = isAdvertising;
	json["isShutdown"] = isShutdown;
	json["isObserver"] = (observer > 0) ? true : false;

	json["ip"] = "192.168.0.47";
	json["port"] = 7777;

	return json;
}