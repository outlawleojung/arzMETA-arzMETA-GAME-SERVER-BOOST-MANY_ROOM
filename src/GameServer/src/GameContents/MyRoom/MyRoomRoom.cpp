#include "MyRoomRoom.h"

#include "../../Session/GameSession.h"
#include "../../ClientPacketHandler.h"

#include "../Game/GameClient.h"

#include "../ClientManager.h"

MyRoomRoom::MyRoomRoom(vector<string> sceneIds) : GameRoom(sceneIds)
{
}

void MyRoomRoom::Init()
{
	state = RoomState::Running;
	
	//this->DoTimer(30000, std::function<void()>(
	//	[this]() {
	//		if (this->state != RoomState::Running)
	//			return;

	//		if (this->clients.size() == 0)
	//			Close();
	//	}
	//));
}
