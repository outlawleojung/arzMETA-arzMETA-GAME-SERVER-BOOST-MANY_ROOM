#include "RoomBase.h"
#include "../RoomManager.h"
#include "ClientBase.h"

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

	GRoomManager->RemoveRoom(roomId);
}

void RoomBase::HandleClose()
{
	for (auto client = clients.begin(); client != clients.end(); client++)
		client->second->DoAsync(&ClientBase::Leave, string("Closing"));

	state = RoomState::Closed;
}

void RoomBase::Leave(shared_ptr<ClientBase> _client)
{
	auto client = clients.find(_client->clientId);
	if (client == clients.end())
		return;

	clients.erase(client);
}

void RoomBase::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	for (auto client = clients.begin(); client != clients.end(); client++)
		client->second->Send(sendBuffer);
}

void RoomBase::Handle_C_LEAVE(shared_ptr<ClientBase>& client, Protocol::C_LEAVE& pkt)
{
	client->DoAsync(&ClientBase::Leave, string("Leaved"));
}