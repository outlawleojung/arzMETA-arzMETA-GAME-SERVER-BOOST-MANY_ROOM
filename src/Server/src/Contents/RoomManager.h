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
	map<string, shared_ptr<RoomBase>> ArzRooms;
	map<string, shared_ptr<RoomBase>> BusanRooms;

	map<string, shared_ptr<RoomBase>> JumpingMatchingRooms;
	map<string, shared_ptr<RoomBase>> OXQuizRooms;

	map<string, shared_ptr<RoomBase>> LectureRooms;
	map<string, shared_ptr<RoomBase>> MeetingRooms;
	map<string, shared_ptr<RoomBase>> ConsultingRooms;

	map<string, shared_ptr<RoomBase>> MyRoomRooms;

	map<string, shared_ptr<RoomBase>> ConferenceRooms;
	map<string, shared_ptr<RoomBase>> GameRooms;
	map<string, shared_ptr<RoomBase>> OfficeRooms;
	map<string, shared_ptr<RoomBase>> StoreRooms;
	map<string, shared_ptr<RoomBase>> VoteRooms;

	map<string, shared_ptr<RoomBase>> HospitalRooms;

	shared_mutex _mutex;
};