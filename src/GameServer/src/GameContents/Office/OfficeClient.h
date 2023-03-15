#pragma once

#include "../Game/GameClient.h"
#include "OfficeRoom.h"

class OfficeClient : public GameClient
{
public:
	OfficeRoomUserType type;
	bool screenPermission;
	bool chatPermission;
	bool voicePermission;
	bool videoPermission;
};