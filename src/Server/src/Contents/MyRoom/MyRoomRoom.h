#pragma once

#include "../Game/GameRoom.h"

class MyRoomRoom : public GameRoom
{
public:
	virtual void Init() override;

	string ownerId;
	string myroomInfo;
};