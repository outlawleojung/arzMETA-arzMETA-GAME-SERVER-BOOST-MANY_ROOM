#pragma once

#include "../Util/pch.h"

#include <boost/thread.hpp>

class RoomBase;

class RoomManager
{
public:
	void AddRoom(shared_ptr<RoomBase> room);
	bool RemoveRoom(string roomId);

	map<string, shared_ptr<RoomBase>> rooms;
	boost::recursive_mutex _mutex;
};