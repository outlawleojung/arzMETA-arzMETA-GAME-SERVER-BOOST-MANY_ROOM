#include "RoomBase.h"
#include "../RoomManager.h"
#include "ClientBase.h"
#include "../../ClientPacketHandler.h"

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

	state = RoomState::Closing;

	HandleClose();

	GRoomManager->RemoveRoom(static_pointer_cast<RoomBase>(shared_from_this()));
}

void RoomBase::HandleClose()
{
	for (auto client = clients.begin(); client != clients.end(); client++)
		client->second->DoAsync(&ClientBase::Leave, string("Closing"));

	state = RoomState::Closed;
}

void RoomBase::Handle_C_LEAVE(shared_ptr<ClientBase>& client, Protocol::C_LEAVE& pkt) { client->DoAsync(&ClientBase::Leave, string("Leaved")); }
void RoomBase::Handle_C_SET_NICKNAME(shared_ptr<ClientBase>& client, Protocol::C_SET_NICKNAME& pkt) { DoAsync(&RoomBase::SetNickname, client, pkt.nickname()); }
void RoomBase::Handle_C_GET_CLIENT(shared_ptr<ClientBase>& client, Protocol::C_GET_CLIENT& pkt) { DoAsync(&RoomBase::GetClient, client); }

void RoomBase::Leave(shared_ptr<ClientBase> _client)
{
	auto client = clients.find(_client->clientId);
	if (client == clients.end())
		return;

	clients.erase(client);

	Protocol::S_REMOVE_CLIENT removeClient;
	removeClient.add_clientids(_client->clientId);
	Broadcast(ClientPacketHandler::MakeSendBuffer(removeClient));

	if (clients.size() == 0)
		Close();
}

void RoomBase::SetNickname(shared_ptr<ClientBase> _client, string nickname)
{
	auto client = clients.find(_client->clientId);
	if (client == clients.end())
		return;

	client->second->nickname = nickname;

	{
		Protocol::S_SET_NICKNAME setNickname;
		setNickname.set_success(true);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(setNickname);
		_client->Send(sendBuffer);
	}

	{
		Protocol::S_SET_NICKNAME_NOTICE setNicknameNotice;
		setNicknameNotice.set_clientid(client->second->clientId);
		setNicknameNotice.set_nickname(nickname);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(setNicknameNotice);
		Broadcast(sendBuffer);
	}
}

void RoomBase::GetClient(shared_ptr<ClientBase> _client)
{
	Protocol::S_ADD_CLIENT res;

	for (auto& client : clients)
	{
		auto clientInfo = res.add_clientinfos();
		clientInfo->set_clientid(client.second->clientId);
		clientInfo->set_nickname(client.second->nickname);
	}

	if (res.clientinfos_size() > 0)
	{
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(res);
		_client->Send(sendBuffer);
	}
}

void RoomBase::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	for (auto client = clients.begin(); client != clients.end(); client++)
		client->second->Send(sendBuffer);
}
