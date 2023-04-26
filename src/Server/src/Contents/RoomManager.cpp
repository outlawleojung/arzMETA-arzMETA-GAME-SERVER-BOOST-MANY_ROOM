#include "../pch.h"
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
    case RoomType::ArzLand:
    {
        arzLandRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::GameZone:
    {
        gameZoneRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::CTFZone:
    {
        ctfZoneRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::VoteZone:
    {
        voteZoneRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::StoreZone:
    {
        storeZoneRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::OfficeLobbyZone:
    {
        officeLobbyZoneRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::BusanLand:
    {
        busanLandRooms.insert({ room->roomId, room });
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
    case RoomType::Meeting:
    {
        meetingRooms.insert({ static_pointer_cast<OfficeRoom>(room)->roomCode, room });
        break;
    }
    case RoomType::Lecture:
    {
        lectureRooms.insert({ static_pointer_cast<OfficeRoom>(room)->roomCode, room });
        break;
    }
    case RoomType::Counsel:
    {
        counselRooms.insert({ static_pointer_cast<OfficeRoom>(room)->roomCode, room });
        break;
    }
    case RoomType::JumpingMatching:
    {
        jumpingMatchingRooms.insert({ static_pointer_cast<MatchingRoom>(room)->roomCode, room });
        break;
    }
    case RoomType::OX:
    {
        oxRooms.insert({ static_pointer_cast<OXRoom>(room)->roomCode, room });
        break;
    }
    default:
        throw std::invalid_argument("Invalid RoomType value");
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
    case RoomType::GameZone:
    {
        gameZoneRooms.erase(room->roomId);
        break;
    }
    case RoomType::CTFZone:
    {
        ctfZoneRooms.erase(room->roomId);
        break;
    }
    case RoomType::VoteZone:
    {
        voteZoneRooms.erase(room->roomId);
        break;
    }
    case RoomType::StoreZone:
    {
        storeZoneRooms.erase(room->roomId);
        break;
    }
    case RoomType::OfficeLobbyZone:
    {
        officeLobbyZoneRooms.erase(room->roomId);
        break;
    }
    case RoomType::BusanLand:
    {
        busanLandRooms.erase(room->roomId);
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
    case RoomType::Meeting:
    {
        meetingRooms.erase(static_pointer_cast<OfficeRoom>(room)->roomCode);
        break;
    }
    case RoomType::Lecture:
    {
        lectureRooms.erase(static_pointer_cast<OfficeRoom>(room)->roomCode);
        break;
    }
    case RoomType::Counsel:
    {
        counselRooms.erase(static_pointer_cast<OfficeRoom>(room)->roomCode);
        break;
    }
    case RoomType::JumpingMatching:
    {
        jumpingMatchingRooms.erase(static_pointer_cast<MatchingRoom>(room)->roomCode);
        break;
    }
    case RoomType::OX:
    {
        oxRooms.erase(static_pointer_cast<OXRoom>(room)->roomCode);
        break;
    }
    default:
        throw std::invalid_argument("Invalid RoomType value");
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
            res.push_back(room->second->roomInfo);

        return res;
    }

    auto type = stringToRoomType(query["type"]);

    switch (type)
    {
    case RoomType::ArzLand:
    {
        for (auto arzLandRoom = arzLandRooms.begin(); arzLandRoom != arzLandRooms.end(); arzLandRoom++)
            res.push_back(arzLandRoom->second->roomInfo);
        break;
    }
    case RoomType::GameZone:
    {
        for (auto gameZoneRoom = gameZoneRooms.begin(); gameZoneRoom != gameZoneRooms.end(); gameZoneRoom++)
            res.push_back(gameZoneRoom->second->roomInfo);
        break;
    }
    case RoomType::CTFZone:
    {
        for (auto ctfZoneRoom = ctfZoneRooms.begin(); ctfZoneRoom != ctfZoneRooms.end(); ctfZoneRoom++)
            res.push_back(ctfZoneRoom->second->roomInfo);
        break;
    }
    case RoomType::VoteZone:
    {
        for (auto voteZoneRoom = voteZoneRooms.begin(); voteZoneRoom != voteZoneRooms.end(); voteZoneRoom++)
            res.push_back(voteZoneRoom->second->roomInfo);
        break;
    }
    case RoomType::StoreZone:
    {
        for (auto storeZoneRoom = storeZoneRooms.begin(); storeZoneRoom != storeZoneRooms.end(); storeZoneRoom++)
            res.push_back(storeZoneRoom->second->roomInfo);
        break;
    }
    case RoomType::OfficeLobbyZone:
    {
        for (auto officeLobbyZoneRoom = officeLobbyZoneRooms.begin(); officeLobbyZoneRoom != officeLobbyZoneRooms.end(); officeLobbyZoneRoom++)
            res.push_back(officeLobbyZoneRoom->second->roomInfo);
        break;
    }
    case RoomType::BusanLand:
    {
        for (auto busanLandRoom = busanLandRooms.begin(); busanLandRoom != busanLandRooms.end(); busanLandRoom++)
            res.push_back(busanLandRoom->second->roomInfo);
        break;
    }
    case RoomType::MyRoom:
    {
        if (query.find("ownerId") != query.end())
        {
            auto myroom = myRoomRooms.find(query["ownerId"]);
            if (myroom != myRoomRooms.end())
                res.push_back(myroom->second->roomInfo);
        }
        else
            for (auto myRoomRoom = myRoomRooms.begin(); myRoomRoom != myRoomRooms.end(); myRoomRoom++)
                res.push_back(myRoomRoom->second->roomInfo);
        break;
    }
    case RoomType::Office:
    {
        if (query.find("roomCode") != query.end())
        {
            auto officeRoom = officeRooms.find(query["roomCode"]);
            if (officeRoom != officeRooms.end())
                res.push_back(officeRoom->second->roomInfo);
        }
        else
            for (auto officeRoom = officeRooms.begin(); officeRoom != officeRooms.end(); officeRoom++)
                res.push_back(officeRoom->second->roomInfo);
        break;
    }
    case RoomType::Meeting:
    {
        if (query.find("roomCode") != query.end())
        {
            auto meetingRoom = meetingRooms.find(query["roomCode"]);
            if (meetingRoom != meetingRooms.end())
                res.push_back(meetingRoom->second->roomInfo);
        }
        else
            for (auto meetingRoom = meetingRooms.begin(); meetingRoom != meetingRooms.end(); meetingRoom++)
                res.push_back(meetingRoom->second->roomInfo);
        break;
    }
    case RoomType::Lecture:
    {
        if (query.find("roomCode") != query.end())
        {
            auto lectureRoom = lectureRooms.find(query["roomCode"]);
            if (lectureRoom != lectureRooms.end())
                res.push_back(lectureRoom->second->roomInfo);
        }
        else
            for (auto lectureRoom = lectureRooms.begin(); lectureRoom != lectureRooms.end(); lectureRoom++)
                res.push_back(lectureRoom->second->roomInfo);
        break;
    }
    case RoomType::Counsel:
    {
        if (query.find("roomCode") != query.end())
        {
            auto counselRoom = counselRooms.find(query["roomCode"]);
            if (counselRoom != counselRooms.end())
                res.push_back(counselRoom->second->roomInfo);
        }
        else
            for (auto counselRoom = counselRooms.begin(); counselRoom != counselRooms.end(); counselRoom++)
                res.push_back(counselRoom->second->roomInfo);
        break;
    }
    case RoomType::JumpingMatching:
    {
        if (query.find("roomCode") != query.end())
        {
            auto jumpingMatchingRoom = jumpingMatchingRooms.find(query["roomCode"]);
            if (jumpingMatchingRoom != jumpingMatchingRooms.end())
                res.push_back(jumpingMatchingRoom->second->roomInfo);
        }
        else
            for (auto matchingRoom = jumpingMatchingRooms.begin(); matchingRoom != jumpingMatchingRooms.end(); matchingRoom++)
                res.push_back(matchingRoom->second->roomInfo);
        break;
    }
    case RoomType::OX:
    {
        if (query.find("roomCode") != query.end())
        {
            auto oxRoom = oxRooms.find(query["roomCode"]);
            if (oxRoom != oxRooms.end())
                res.push_back(oxRoom->second->roomInfo);
        }
        else
            for (auto oxRoom = oxRooms.begin(); oxRoom != oxRooms.end(); oxRoom++)
                res.push_back(oxRoom->second->roomInfo);
        break;
    }
    default:
        throw std::invalid_argument("Invalid RoomType value");
    }

    return res;
}
