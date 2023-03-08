#include "OfficeRoom.h"
//#include <httplib.h>

#include "../ClientPacketHandler.h"

#include "../Session/GameSession.h"

OfficeRoom::OfficeRoom(vector<string> sceneIds)
	: GameRoom(sceneIds)
	, currentPersonnel(0)
	, currentObserver(0)
	, isWaitingRoom(false)
	, isPassword(false)
	, isAdvertising(false)
	, isShutdown(false)
	, passedTime(0)
{}

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
		client->second.second->session->Disconnect("CLOSING");

	for (auto client = clients.begin(); client != clients.end(); client++)
		client->second->session->Disconnect("CLOSING");

	//waitingList.clear();
	//clients.clear();

	GameRoom::HandleClose();

	state = RoomState::Closed;

	//Main 의 http 서버의 종료, 이걸 여기서 해야 하나?
	//httplib::Client httpClient(localHostIp, 8080);
	//httpClient.Post("/Stop");
}

void OfficeRoom::Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) { DoAsync(&OfficeRoom::Enter, session, pkt); }
void OfficeRoom::Handle_C_REENTER(shared_ptr<GameSession>& session, Protocol::C_REENTER& pkt) { DoAsync(&OfficeRoom::ReEnter, session, pkt.clientid()); }
void OfficeRoom::Handle_C_LEAVE(shared_ptr<GameSession>& session, Protocol::C_LEAVE& pkt) { DoAsync(&OfficeRoom::Leave, session); }
void OfficeRoom::Handle_C_SET_NICKNAME(shared_ptr<GameSession>& session, Protocol::C_SET_NICKNAME& pkt) { DoAsync(&OfficeRoom::SetNickname, session, pkt.nickname()); }

void OfficeRoom::Handle_C_BASE_SET_SCENE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_SCENE& pkt) { DoAsync(&OfficeRoom::SetScene, session, pkt.sceneid()); }
void OfficeRoom::Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt) { DoAsync(&OfficeRoom::InstantiateObject, session, pkt); }

void OfficeRoom::Handle_C_INTERACTION_SET_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_SET_ITEM& pkt)
{
}

void OfficeRoom::Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_WAITING_LIST& pkt) { DoAsync(&OfficeRoom::GetWaitingList, session); }
void OfficeRoom::Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_ACCEPT_WAIT& pkt) { DoAsync(&OfficeRoom::AcceptWait, session, pkt.clientid(), pkt.isaccepted()); } 
void OfficeRoom::Handle_C_OFFICE_GET_CLIENT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_CLIENT& pkt) { DoAsync(&OfficeRoom::GetClient, session); }
void OfficeRoom::Handle_C_OFFICE_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_HOST& pkt) { DoAsync(&OfficeRoom::GetHost, session); }
void OfficeRoom::Handle_C_OFFICE_BREAK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_BREAK& pkt) { DoAsync(&OfficeRoom::Break, session); }
void OfficeRoom::Handle_C_OFFICE_KICK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_KICK& pkt) { DoAsync(&OfficeRoom::Kick, session, pkt.clientid()); }
void OfficeRoom::Handle_C_OFFICE_GET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_PERMISSION& pkt) { DoAsync(&OfficeRoom::GetPermission, session, pkt.clientid()); }
void OfficeRoom::Handle_C_OFFICE_SET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_PERMISSION& pkt) { DoAsync(&OfficeRoom::SetPermission, session, pkt); }
void OfficeRoom::Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_ROOM_INFO& pkt) { DoAsync(&OfficeRoom::SetRoomInfo, session, pkt); }
void OfficeRoom::Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_ROOM_INFO& pkt) { DoAsync(&OfficeRoom::GetRoomInfo, session); }
void OfficeRoom::Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<GameSession>& session, Protocol::C_OFFICE_VIDEO_STREAM& pkt) { DoAsync(&OfficeRoom::HandleVideoStream, pkt.url(), pkt.volume(), pkt.time(), pkt.play(), pkt.seek(), pkt.mediaplayerstate()); }

void OfficeRoom::Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt)
{
	Protocol::S_ENTER res;

	//중복 로그인 체크
	if (waitingList.find(pkt.clientid()) != waitingList.end()
		|| clients.find(pkt.clientid()) != clients.end())
	{
		res.set_result("DUPLICATED");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		session->Disconnect("ENTER_FAIL");
	}

	GLogManager->Log("Session Try to Enter :		", pkt.clientid());

	auto officeRoomSession = make_shared<OfficeRoomSession>();
	officeRoomSession->session = session;
	if(pkt.isobserver())
		officeRoomSession->type = OfficeRoomUserType::Observer;
	else
		if (modeType == 1)
			officeRoomSession->type = OfficeRoomUserType::Guest;
		else if (modeType == 2)
			officeRoomSession->type = OfficeRoomUserType::Audience;

	if (clients.size() == 0 && creatorId == pkt.clientid())
	{
		res.set_result("SUCCESS");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		officeRoomSession->type = OfficeRoomUserType::Host;
		clients.insert({ pkt.clientid(), officeRoomSession });

		currentPersonnel++;

		for (auto it = C_ENTER_Handlers.begin(); it != C_ENTER_Handlers.end(); it++)
			(*it)(session, pkt);

		return;
	}

	if (clients.size() == 0)
	{
		res.set_result("HOST_NOT_ENTERED");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		session->Disconnect("ENTER_FAIL");
		return;
	}

	if (isShutdown)
	{
		res.set_result("ROOM_IS_SHUTDONW");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		session->Disconnect("ENTER_FAIL");
		return;
	}

	if (isPassword && password != pkt.password())
	{
		res.set_result("PASSWORD_FAIL");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		session->Disconnect("ENTER_FAIL");
		return;
	}

	if (isWaitingRoom)
	{
		res.set_result("WAITING");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		
		waitingList.insert({ pkt.clientid(), { pkt.isobserver(),officeRoomSession } });

		Protocol::S_OFFICE_ADD_WAITING_CLIENT addWaitingClient;
		auto waitingClient = addWaitingClient.add_clients();
		waitingClient->set_isobserver(pkt.isobserver());
		waitingClient->set_clientid(pkt.clientid());
		waitingClient->set_nickname(pkt.nickname());
		clients.find(currentHostId)->second->session->Send(ClientPacketHandler::MakeSendBuffer(addWaitingClient));

		return;	
	}

	if ((!pkt.isobserver() && (personnel <= currentPersonnel)) || (pkt.isobserver() && (observer <= currentObserver)))
	{
		res.set_result("ROOM_IS_FULL");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		session->Disconnect("ENTER_FAIL");
		return;
	}

	res.set_result("SUCCESS");
	session->Send(ClientPacketHandler::MakeSendBuffer(res));

	clients.insert({ pkt.clientid(), officeRoomSession });
		
	if (pkt.isobserver())
		currentObserver++;
	else
		currentPersonnel++;
			
	for (auto it = C_ENTER_Handlers.begin(); it != C_ENTER_Handlers.end(); it++)
		(*it)(session, pkt);
}

void OfficeRoom::ReEnter(shared_ptr<GameSession> session, string clientId)
{	
}

void OfficeRoom::Leave(shared_ptr<GameSession> session)
{
	session->Disconnect("LEAVED");

	//대기열 먼저 확인, 존재했으면 지우고 호스트에게 알림
	{
		auto client = waitingList.find(session->clientId);
		if (client != waitingList.end())
		{
			Protocol::S_OFFICE_REMOVE_WAITING_CLIENT removeWaitingClient;
			removeWaitingClient.add_clients(client->second.second->session->clientId);
			clients.find(currentHostId)->second->session->Send(ClientPacketHandler::MakeSendBuffer(removeWaitingClient));

			waitingList.erase(client);
			return;
		}
	}
	
	//대기열에도 없고, clients 에도 없으면 거기에서 끝
	auto client = clients.find(session->clientId);
	if (client == clients.end())
		return;

	//인원수 조정
	if (client->second->type == OfficeRoomUserType::Observer)
		currentObserver--;
	else
		currentPersonnel--;
	
	//clients 에서 삭제
	clients.erase(client);

	GameRoom::Leave(session);

	GLogManager->Log("Session Leaved :			", session->clientId);

	if (DESTROY_WHEN_EMPTY)
		if (clients.size() == 0 || session->clientId == currentHostId)
			Close();
}

void OfficeRoom::SetNickname(shared_ptr<GameSession> session, string nickname)
{
	auto client = clients.find(session->clientId);
	if (client == clients.end())
		return;

	client->second->session->nickname = nickname;

	{
		Protocol::S_SET_NICKNAME setNickname;
		setNickname.set_success(true);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(setNickname);
		session->Send(sendBuffer);
	}

	{
		Protocol::S_SET_NICKNAME_NOTICE setNicknameNotice;
		setNicknameNotice.set_clientid(session->clientId);
		setNicknameNotice.set_nickname(nickname);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(setNicknameNotice);
		this->DoAsync(&OfficeRoom::Broadcast, sendBuffer);
	}
}

void OfficeRoom::GetClient(shared_ptr<GameSession> session)
{
	Protocol::S_OFFICE_ADD_CLIENT res;

	for (auto& client : clients)
	{
		auto clientInfo = res.add_clientinfos();
		clientInfo->set_clientid(client.second->session->clientId);
		clientInfo->set_nickname(client.second->session->nickname);
	}

	if (res.clientinfos_size() > 0)
	{
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(res);
		session->Send(sendBuffer);
	}
}

void OfficeRoom::GetHost(shared_ptr<GameSession> session)
{
	Protocol::S_OFFICE_GET_HOST res;
	res.set_clientid(currentHostId);
	session->Send(ClientPacketHandler::MakeSendBuffer(res));
}

void OfficeRoom::Break(shared_ptr<GameSession> session)
{
	if (session->clientId != currentHostId) return;

	Protocol::S_OFFICE_BREAK res;
	res.set_success(true);
	session->Send(ClientPacketHandler::MakeSendBuffer(res));

	Close();
}

void OfficeRoom::Kick(shared_ptr<GameSession> session, string clientId)
{
	if (session->clientId != currentHostId) return;

	Protocol::S_OFFICE_KICK res;

	//존재하는지 확인
	auto kickedClient = clients.find(clientId);
	if (kickedClient == clients.end())
	{
		res.set_success(false);
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		return;
	}

	if (!kickedClient->second->session->Disconnect("KICKED"))
	{
		res.set_success(false);
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		return;
	}

	res.set_success(true);
	session->Send(ClientPacketHandler::MakeSendBuffer(res));
}

void OfficeRoom::GetPermission(shared_ptr<GameSession> session, string clientId)
{
	if (session->clientId != currentHostId) return;

	Protocol::S_OFFICE_GET_PERMISSION getPermission;

	if (clientId.empty())
	{
		for (auto client = clients.begin(); client != clients.end(); client++)
		{
			auto permission = getPermission.add_permissions();
			permission->set_clientid(client->second->session->clientId);
			permission->set_screenpermission(client->second->screenPermission);
			permission->set_chatpermission(client->second->chatPermission);
			permission->set_voicepermission(client->second->voicePermission);
			permission->set_videopermission(client->second->videoPermission);
			permission->set_type(client->second->type);
		}
	}
	else
	{
		auto client = clients.find(clientId);
		if (client != clients.end())
		{
			auto permission = getPermission.add_permissions();
			permission->set_clientid(client->second->session->clientId);
			permission->set_screenpermission(client->second->screenPermission);
			permission->set_chatpermission(client->second->chatPermission);
			permission->set_voicepermission(client->second->voicePermission);
			permission->set_videopermission(client->second->videoPermission);
			permission->set_type(client->second->type);
		}
	}

	if (getPermission.permissions_size() > 0)
	{
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(getPermission);
		session->Send(sendBuffer);
	}
}

void OfficeRoom::SetPermission(shared_ptr<GameSession> session, Protocol::C_OFFICE_SET_PERMISSION pkt)
{
	if (session->clientId != currentHostId) return;

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

			client->second->screenPermission = permission.screenpermission();
			client->second->chatPermission = permission.chatpermission();
			client->second->voicePermission = permission.voicepermission();
			client->second->videoPermission = permission.videopermission();
			client->second->type = static_cast<OfficeRoomUserType>(permission.type());

			if (client->second->type == OfficeRoomUserType::Host)
				currentHostId = client->second->session->clientId;
		}
	}

	//문제가 없었을 경우

	{
		Protocol::S_OFFICE_SET_PERMISSION permission;
		permission.set_success(true);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(permission);
		session->Send(sendBuffer);
	}

	{
		for (int i = 0; i < pkt.permissions_size(); i++)
		{
			auto client = clients.find(pkt.permissions().Get(i).clientid());
			if (client == clients.end())
				continue;

			Protocol::S_OFFICE_SET_PERMISSION_NOTICE permissionNotice;

			permissionNotice.set_screenpermission(client->second->screenPermission);
			permissionNotice.set_chatpermission(client->second->chatPermission);
			permissionNotice.set_voicepermission(client->second->voicePermission);
			permissionNotice.set_videopermission(client->second->videoPermission);
			permissionNotice.set_type(client->second->type);

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(permissionNotice);
			client->second->session->Send(sendBuffer);
		}
	}
}

void OfficeRoom::GetRoomInfo(shared_ptr<GameSession> session)
{
	Protocol::S_OFFICE_GET_ROOM_INFO roomInfo;

	roomInfo.set_roomname(roomName);
	roomInfo.set_description(description);
	roomInfo.set_topictype(topicType);
	roomInfo.set_password(password);
	roomInfo.set_spaceinfoid(spaceInfoId);
	roomInfo.set_personnel(personnel);
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
		roomInfo.set_hostnickname(host->second->session->nickname);

	session->Send(ClientPacketHandler::MakeSendBuffer(roomInfo));
}

void OfficeRoom::SetRoomInfo(shared_ptr<GameSession> session, Protocol::C_OFFICE_SET_ROOM_INFO pkt)
{
	if (session->clientId != currentHostId) return;

	personnel = pkt.personnel();
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
	session->Send(ClientPacketHandler::MakeSendBuffer(roomInfo));
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

void OfficeRoom::GetWaitingList(shared_ptr<GameSession> session)
{
	Protocol::S_OFFICE_ADD_WAITING_CLIENT waitingListPkt;
	for (auto wait = waitingList.begin(); wait != waitingList.end(); wait++)
	{
		auto client = waitingListPkt.add_clients();
		client->set_isobserver(wait->second.first);
		client->set_clientid(wait->second.second->session->clientId);
		client->set_nickname(wait->second.second->session->nickname);
	}

	session->Send(ClientPacketHandler::MakeSendBuffer(waitingListPkt));
}

void OfficeRoom::AcceptWait(shared_ptr<GameSession> session, string clientId, bool isAccepted)
{
	if (session->clientId != currentHostId) return;

	Protocol::S_OFFICE_ACCEPT_WAIT acceptWait;

	auto client = waitingList.find(clientId);
	if (client == waitingList.end())
	{
		acceptWait.set_success(false);
		//set result as "no client with clientid ~"
		session->Send(ClientPacketHandler::MakeSendBuffer(acceptWait));
		return;
	}
	
	//인원이 꽉 찬 상태에서 수락하려고 하는 경우 실패 처리
	if ( !client->second.first && personnel <= currentPersonnel
		|| client->second.first && observer <= currentObserver)
	{
		if (isAccepted)
		{
			acceptWait.set_success(false);
			//set result as "room is full"
			session->Send(ClientPacketHandler::MakeSendBuffer(acceptWait));
			return;
		}
	}

	//이외의 경우 성공, 호스트에게 관련 메시지 전송
	acceptWait.set_success(true);
	session->Send(ClientPacketHandler::MakeSendBuffer(acceptWait));

	Protocol::S_OFFICE_REMOVE_WAITING_CLIENT removeWaitingClient;
	removeWaitingClient.add_clients(client->second.second->session->clientId);
	session->Send(ClientPacketHandler::MakeSendBuffer(removeWaitingClient));

	//대상 클라이언트에게 성공/실패 메시지 전송
	Protocol::S_OFFICE_ACCEPT_WAIT_NOTICE acceptWaitNotice;
	acceptWaitNotice.set_isaccepted(isAccepted);
	client->second.second->session->Send(ClientPacketHandler::MakeSendBuffer(acceptWaitNotice));

	//입장 허락이었을 경우의 처리, enter 시의 처리와 동일
	if (isAccepted)
	{
		clients.insert({ client->second.second->session->clientId, client->second.second });

		if (client->second.first)
			currentObserver++;
		else
			currentPersonnel++;
	}

	//대기열에서 대상 클라이언트 삭제
	waitingList.erase(client);
}

void OfficeRoom::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	for (auto client = clients.begin(); client != clients.end(); client++)
		client->second->session->Send(sendBuffer);
}

void OfficeRoom::SetScene(shared_ptr<GameSession> session, string sceneId)
{
	auto client = clients.find(session->clientId);
	if (client == clients.end())
		return;

	GameRoom::SetScene(session, sceneId);
}

void OfficeRoom::InstantiateObject(shared_ptr<GameSession> session, Protocol::C_BASE_INSTANTIATE_OBJECT pkt)
{
	auto client = clients.find(session->clientId);
	if (client == clients.end())
		return;

	if (client->second->type == OfficeRoomUserType::Observer)
		return;

	GameRoom::Handle_C_BASE_INSTANTIATE_OBJECT(session, pkt);
}

void OfficeRoom::SetState(shared_ptr<GameSession> session, Protocol::C_INTERACTION_SET_ITEM pkt)
{
	auto client = clients.find(session->clientId);
	if (client == clients.end())
		return;

	if (client->second->type == OfficeRoomUserType::Observer)
		return;

	GameRoom::Handle_C_INTERACTION_SET_ITEM(session, pkt);
}

void OfficeRoom::RemoveState(shared_ptr<GameSession> session, Protocol::C_INTERACTION_REMOVE_ITEM pkt)
{
	auto client = clients.find(session->clientId);
	if (client == clients.end())
		return;

	if (client->second->type == OfficeRoomUserType::Observer)
		return;

	GameRoom::Handle_C_INTERACTION_REMOVE_ITEM(session, pkt);
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

	/*nlohmann::json modulesJson = nlohmann::json::array();

	for (auto module = modules.begin(); module != modules.end(); module++)
	{
		auto moduleJson = (*module)->ToJson();
		if (!moduleJson.empty())
			modulesJson.push_back(moduleJson);
		else
			return nlohmann::json::object();
	}

	json["Modules"] = modulesJson;

	json["roomName"] = roomName;
	json["roomCode"] = roomId;
	json["description"] = description;
	json["spaceInfoId"] = spaceInfoId;
	json["modeType"] = modeType;
	json["topicType"] = topicType;
	json["thumbnail"] = thumbnail;
	
	auto host = clients.find(hostId);
	if (host != clients.end())
		json["host"] = host->second->session->nickname;
	else
		json["host"] = "";

	json["personnel"] = personnel;
	json["currentPersonnel"] = currentPersonnel;
	json["isPassword"] = isPassword;
	json["isAdvertising"] = isAdvertising;
	json["isShutdown"] = isShutdown;
	json["isObserver"] = (observer > 0) ? true : false;

	cout << "test : " << json.dump() << endl;*/

	return json;
}