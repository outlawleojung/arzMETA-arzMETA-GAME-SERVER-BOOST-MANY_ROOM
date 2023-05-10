#pragma once

#include "../Game/GameClient.h"
#include "MeetingRoom.h"

class MeetingClient : public GameClient
{
public:
	MeetingRoomUserType type;
	bool screenPermission;
	bool chatPermission;
	bool voicePermission;
	bool videoPermission;
};