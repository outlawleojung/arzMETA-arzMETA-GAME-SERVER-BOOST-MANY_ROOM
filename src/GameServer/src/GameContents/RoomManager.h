#pragma once

#include "../Util/pch.h"
#include "nlohmann/json.hpp"

#include <shared_mutex>

class RoomBase;

class RoomManager
{
public:
	void AddRoom(shared_ptr<RoomBase> room);
	bool RemoveRoom(string roomId);
	nlohmann::json GetRoom(map<string, string> query);

	map<string, shared_ptr<RoomBase>> rooms;
	map<string, shared_ptr<RoomBase>> visibleRooms;
	shared_mutex _mutex;
};