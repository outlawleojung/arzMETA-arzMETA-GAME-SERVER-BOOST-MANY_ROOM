#pragma once

#include "../Game/GameClient.h"
#include "LectureRoom.h"

class LectureClient : public GameClient
{
public:
	LectureRoomUserData data;
};