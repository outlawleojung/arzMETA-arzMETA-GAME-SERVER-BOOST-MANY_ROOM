#pragma once

#include "../Util/pch.h"
#include "nlohmann/json.hpp"

#include "RoomTypes.h"

#include <shared_mutex>

class RoomBase;

class RoomManager
{
public:
	void AddRoom(shared_ptr<RoomBase> room);
	void IndexRoom(shared_ptr<RoomBase> room);
	bool RemoveRoom(shared_ptr<RoomBase> room);
	nlohmann::json GetRoom(map<string, string> query);

	//all rooms
	map<string, shared_ptr<RoomBase>> rooms;

	//indexed rooms
	map<string, shared_ptr<RoomBase>> arzLandRooms; //room id as key
	map<string, shared_ptr<RoomBase>> myRoomRooms; //owner id as key
	map<string, shared_ptr<RoomBase>> officeRooms; //Room code as key
	map<string, shared_ptr<RoomBase>> matchingRooms; //Room code as key
	map<string, shared_ptr<RoomBase>> oxRooms; //Room code as key

	shared_mutex _mutex;
};