#pragma once

#include "../pch.h"
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
    map<string, shared_ptr<RoomBase>> arzLandRooms;
    map<string, shared_ptr<RoomBase>> gameZoneRooms;
    map<string, shared_ptr<RoomBase>> ctfZoneRooms;
    map<string, shared_ptr<RoomBase>> voteZoneRooms;
    map<string, shared_ptr<RoomBase>> storeZoneRooms;
    map<string, shared_ptr<RoomBase>> officeLobbyZoneRooms;
    map<string, shared_ptr<RoomBase>> busanLandRooms;
    map<string, shared_ptr<RoomBase>> myRoomRooms;
    map<string, shared_ptr<RoomBase>> officeRooms;
    map<string, shared_ptr<RoomBase>> meetingRooms;
    map<string, shared_ptr<RoomBase>> lectureRooms;
    map<string, shared_ptr<RoomBase>> counselRooms;
    map<string, shared_ptr<RoomBase>> jumpingMatchingRooms;
    map<string, shared_ptr<RoomBase>> oxRooms;

	shared_mutex _mutex;
};