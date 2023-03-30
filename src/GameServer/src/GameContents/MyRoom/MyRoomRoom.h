#pragma once

#include "../Game/GameRoom.h"

class MyRoomRoom : public GameRoom
{
public:
	MyRoomRoom(vector<string> sceneIds);

	virtual void Init() override;

	string ownerId;
	string roomInfo;
};