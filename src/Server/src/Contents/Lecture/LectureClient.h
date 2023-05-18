#pragma once

#include "../Game/GameClient.h"
#include "LectureRoom.h"

class LectureClient : public GameClient
{
public:
	LectureRoomUserType type;
	bool screenPermission;
	bool chatPermission;
	bool voicePermission;
	bool videoPermission;
	bool movePermission;
};