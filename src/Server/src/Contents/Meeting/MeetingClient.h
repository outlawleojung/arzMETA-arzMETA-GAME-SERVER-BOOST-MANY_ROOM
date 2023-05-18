#pragma once

#include "../Game/GameClient.h"
#include "MeetingRoom.h"

class MeetingClient : public GameClient
{
public:
	MeetingRoomUserData data;
};