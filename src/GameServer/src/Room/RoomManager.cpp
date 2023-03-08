#include "../Util/pch.h"
#include "RoomManager.h"
#include "RoomBase.h"

void RoomManager::AddRoom(shared_ptr<RoomBase> room)
{
    boost::lock_guard<boost::recursive_mutex> lock(_mutex);

    static int idGenerator = 0;
    room->roomId = to_string(idGenerator++);

    rooms.insert({ room->roomId, room });

    GLogManager->Log("Total Room Number :	        	", to_string(rooms.size()));
}

bool RoomManager::RemoveRoom(string roomId)
{
    boost::lock_guard<boost::recursive_mutex> lock(_mutex);

    auto room = rooms.find(roomId);
    if (room == rooms.end())
        return false;

    rooms.erase(room);

    GLogManager->Log("Total Room Number :	         	", to_string(rooms.size()));

    return true;
}
