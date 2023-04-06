#include "MyRoomRoom.h"

#include "../../Session/GameSession.h"
#include "../../ClientPacketHandler.h"

#include "../Game/GameClient.h"

#include "../ClientManager.h"

#include "../RoomManager.h"

MyRoomRoom::MyRoomRoom(vector<string> sceneIds) : GameRoom(sceneIds)
{
}

void MyRoomRoom::Init()
{
	state = RoomState::Running;
	
	GRoomManager->IndexRoom(static_pointer_cast<RoomBase>(shared_from_this()));

	//this->DoTimer(30000, std::function<void()>(
	//	[this]() {
	//		if (this->state != RoomState::Running)
	//			return;

	//		if (this->clients.size() == 0)
	//			Close();
	//	}
	//));
}
