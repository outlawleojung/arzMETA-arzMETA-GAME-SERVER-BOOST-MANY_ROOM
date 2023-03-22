#include "../Util/pch.h"
#include "RoomManager.h"
#include "Base/RoomBase.h"
#include "MyRoom/MyRoomRoom.h"

void RoomManager::AddRoom(shared_ptr<RoomBase> room)
{
    boost::lock_guard<boost::mutex> lock(_mutex);

    static int idGenerator = 0;
    room->roomId = to_string(idGenerator++);

    rooms.insert({ room->roomId, room });

    GLogManager->Log("Total Room Number :	        	", to_string(rooms.size()));
}

bool RoomManager::RemoveRoom(string roomId)
{
    boost::lock_guard<boost::mutex> lock(_mutex);

    auto room = rooms.find(roomId);
    if (room == rooms.end())
        return false;

    rooms.erase(room);

    GLogManager->Log("Total Room Number :	         	", to_string(rooms.size()));

    return true;
}

nlohmann::json RoomManager::GetRoom(map<string, string> query)
{
    boost::lock_guard<boost::mutex> lock(_mutex);

    nlohmann::json res = nlohmann::json::array();

    if (query.size() == 0)
    {
        for (auto room = rooms.begin(); room != rooms.end(); room++)
            res.push_back(room->second->ToJson());

        return res;
    }

    for (auto room = rooms.begin(); room != rooms.end(); room++)
    {
        if (query["type"] == "Main")
        {
            if (room->second->type != RoomType::Main)
                continue;

            res.push_back(room->second->ToJson());
        }
        else if (query["type"] == "Office")
        {
            if (room->second->type != RoomType::Office)
                continue;

            res.push_back(room->second->ToJson());
        }
        else if (query["type"] == "MyRoom")
        {
            if (room->second->type != RoomType::MyRoom)
                continue;

            auto myRoom = static_pointer_cast<MyRoomRoom>(room->second);

            if (query.find("Owner") != query.end() && myRoom->ownerId != query["Owner"])
                continue;

            res.push_back(room->second->ToJson());
        }
    }

    return res;
}
