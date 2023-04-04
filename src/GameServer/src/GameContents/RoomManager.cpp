#include "../Util/pch.h"
#include "RoomManager.h"
#include "Base/RoomBase.h"
#include "MyRoom/MyRoomRoom.h"
#include "Office/OfficeRoom.h"
#include "Matching/MatchingRoom.h"

void RoomManager::AddRoom(shared_ptr<RoomBase> room)
{
    std::unique_lock<shared_mutex> lock;

    static int idGenerator = 0;
    room->roomId = to_string(idGenerator++);

    rooms.insert({ room->roomId, room });

    GLogManager->Log("Total Room Number :	        	", to_string(rooms.size()));
}

bool RoomManager::RemoveRoom(string roomId)
{
    std::unique_lock<shared_mutex> lock;

    auto room = rooms.find(roomId);
    if (room == rooms.end())
        return false;

    rooms.erase(room);

    GLogManager->Log("Total Room Number :	         	", to_string(rooms.size()));

    return true;
}

nlohmann::json RoomManager::GetRoom(map<string, string> query)
{
    std::shared_lock<shared_mutex> lock;

    nlohmann::json res = nlohmann::json::array();

    if (query.size() == 0)
    {
        for (auto room = rooms.begin(); room != rooms.end(); room++)
            res.push_back(room->second->ToJson());

        return res;
    }

    for (auto room = rooms.begin(); room != rooms.end(); room++)
    {
        if (query["type"] == "ArzLand")
        {
            if (room->second->type != RoomType::ArzLand)
                continue;

            res.push_back(room->second->ToJson());
        }
        else if (query["type"] == "Office")
        {
            if (room->second->type != RoomType::Office)
                continue;

            auto officeRoom = static_pointer_cast<OfficeRoom>(room->second);

            if (query.find("RoomCode") != query.end() && officeRoom->roomCode != query["RoomCode"])
                continue;

            res.push_back(room->second->ToJson());
        }
        else if (query["type"] == "MyRoom")
        {
            if (room->second->type != RoomType::MyRoom)
                continue;

            auto myRoom = static_pointer_cast<MyRoomRoom>(room->second);

            if (query.find("ownerId") != query.end() && myRoom->ownerId != query["ownerId"])
                continue;

            res.push_back(room->second->ToJson());
        }
        else if (query["type"] == "JumpingMatching")
        {
            if (room->second->type != RoomType::Matching)
                continue;

            auto myRoom = static_pointer_cast<MatchingRoom>(room->second);

            res.push_back(room->second->ToJson());
        }
    }

    return res;
}
