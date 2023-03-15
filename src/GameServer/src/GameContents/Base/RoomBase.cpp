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

void RoomBase::Handle_C_LEAVE(shared_ptr<ClientBase>& client, Protocol::C_LEAVE& pkt)
{
	client->DoAsync(&ClientBase::Leave, string("Leaved"));
}