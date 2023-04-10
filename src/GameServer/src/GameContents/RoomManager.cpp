#include "../Util/pch.h"
#include "RoomManager.h"
#include "Base/RoomBase.h"
#include "MyRoom/MyRoomRoom.h"
#include "Office/OfficeRoom.h"
#include "Matching/MatchingRoom.h"
#include "OX/OXRoom.h"

void RoomManager::AddRoom(shared_ptr<RoomBase> room)
{
    std::unique_lock<shared_mutex> lock;

    static int idGenerator = 0;
    room->roomId = to_string(idGenerator++);

    rooms.insert({ room->roomId, room });

    GLogManager->Log("Total Room Number :	        	", to_string(rooms.size()));
}

void RoomManager::IndexRoom(shared_ptr<RoomBase> room)
{
    std::unique_lock<shared_mutex> lock;

    switch (room->type)
    {
    case RoomType::ArzLand :
    {
        arzLandRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::MyRoom:
    {
        myRoomRooms.insert({ static_pointer_cast<MyRoomRoom>(room)->ownerId, room });
        break;
    }
    case RoomType::Office:
    {
        officeRooms.insert({ static_pointer_cast<OfficeRoom>(room)->roomCode, room });
        break;
    }
    case RoomType::Matching:
    {
        matchingRooms.insert({ static_pointer_cast<MatchingRoom>(room)->roomCode, room });
        break;
    }
    case RoomType::OX:
    {
        oxRooms.insert({ static_pointer_cast<OXRoom>(room)->roomCode, room });
        break;
    }
    default:
        break;
    }
}

bool RoomManager::RemoveRoom(shared_ptr<RoomBase> room)
{
    std::unique_lock<shared_mutex> lock;

    rooms.erase(rooms.find(room->roomId));

    switch (room->type)
    {
    case RoomType::ArzLand:
    {
        arzLandRooms.erase(room->roomId);
        break;
    }
    case RoomType::MyRoom:
    {
        myRoomRooms.erase(static_pointer_cast<MyRoomRoom>(room)->ownerId);
        break;
    }
    case RoomType::Office:
    {
        officeRooms.erase(static_pointer_cast<OfficeRoom>(room)->roomCode);
        break;
    }
    case RoomType::Matching:
    {
        matchingRooms.erase(static_pointer_cast<MatchingRoom>(room)->roomCode);
        break;
    }
    case RoomType::OX:
    {
        oxRooms.erase(static_pointer_cast<OXRoom>(room)->roomCode);
        break;
    }
    default:
        break;
    }

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
            res.push_back(room->second->roomInfoJson);

        return res;
    }

    if (query["type"] == "ArzLand")
    {
        for (auto arzLandRoom = arzLandRooms.begin(); arzLandRoom != arzLandRooms.end(); arzLandRoom++)
            res.push_back(arzLandRoom->second->roomInfoJson);
    }
    else if (query["type"] == "MyRoom")
    {
        if (query.find("ownerId") != query.end() && myRoomRooms.find(query["ownerId"]) != myRoomRooms.end())
            res.push_back(myRoomRooms.find(query["ownerId"])->second->roomInfoJson);
        else
            for (auto myRoomRoom = myRoomRooms.begin(); myRoomRoom != myRoomRooms.end(); myRoomRoom++)
                res.push_back(myRoomRoom->second->roomInfoJson);
    }
    else if (query["type"] == "Office")
    {
        if (query.find("roomCode") != query.end() && officeRooms.find(query["roomCode"]) != officeRooms.end())
            res.push_back(officeRooms.find(query["roomCode"])->second->roomInfoJson);
        else
            for (auto officeRoom = officeRooms.begin(); officeRoom != officeRooms.end(); officeRoom++)
                res.push_back(officeRoom->second->roomInfoJson);
    }
    else if (query["type"] == "JumpingMatching")
    {
        if (query.find("roomCode") != query.end() && matchingRooms.find(query["roomCode"]) != matchingRooms.end())
            res.push_back(matchingRooms.find(query["roomCode"])->second->roomInfoJson);
        else
            for (auto matchingRoom = matchingRooms.begin(); matchingRoom != matchingRooms.end(); matchingRoom++)
                res.push_back(matchingRoom->second->roomInfoJson);

    }
    else if (query["type"] == "OX")
    {
        if (query.find("roomCode") != query.end() && oxRooms.find(query["roomCode"]) != oxRooms.end())
            res.push_back(oxRooms.find(query["roomCode"])->second->roomInfoJson);
        else
            for (auto oxRoom = oxRooms.begin(); oxRoom != oxRooms.end(); oxRoom++)
                res.push_back(oxRoom->second->roomInfoJson);
    }

    return res;
}
