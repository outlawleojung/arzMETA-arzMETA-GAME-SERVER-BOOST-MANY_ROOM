#include "../pch.h"
#include "RoomManager.h"
#include "Base/RoomBase.h"
#include "MyRoom/MyRoomRoom.h"
#include "Office/OfficeRoom.h"
#include "Matching/MatchingRoom.h"
#include "Meeting/MeetingRoom.h"
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
    case RoomType::Arz:
    {
        ArzRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::Busan:
    {
        BusanRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::Conference:
    {
        ConferenceRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::Game:
    {
        GameRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::Office:
    {
        OfficeRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::Store:
    {
        StoreRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::Vote:
    {
        VoteRooms.insert({ room->roomId, room });
        break;
    }
    case RoomType::MyRoom:
    {
        MyRoomRooms.insert({ static_pointer_cast<MyRoomRoom>(room)->ownerId, room });
        break;
    }
    case RoomType::Meeting:
    {
        MeetingRooms.insert({ static_pointer_cast<MeetingRoom>(room)->roomCode, room });
        break;
    }
    case RoomType::Lecture:
    {
        LectureRooms.insert({ static_pointer_cast<OfficeRoom>(room)->roomCode, room });
        break;
    }
    case RoomType::Consulting:
    {
        ConsultingRooms.insert({ static_pointer_cast<MeetingRoom>(room)->roomCode, room });
        break;
    }
    case RoomType::JumpingMatching:
    {
        JumpingMatchingRooms.insert({ static_pointer_cast<MatchingRoom>(room)->roomCode, room });
        break;
    }
    case RoomType::OXQuiz:
    {
        OXQuizRooms.insert({ static_pointer_cast<OXRoom>(room)->roomCode, room });
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
    case RoomType::Arz:
    {
        ArzRooms.erase(room->roomId);
        break;
    }
    case RoomType::Busan:
    {
        BusanRooms.erase(room->roomId);
        break;
    }
    case RoomType::Conference:
    {
        ConferenceRooms.erase(room->roomId);
        break;
    }
    case RoomType::Game:
    {
        GameRooms.erase(room->roomId);
        break;
    }
    case RoomType::Office:
    {
        OfficeRooms.erase(room->roomId);
        break;
    }
    case RoomType::Store:
    {
        StoreRooms.erase(room->roomId);
        break;
    }
    case RoomType::Vote:
    {
        VoteRooms.erase(room->roomId);
        break;
    }
    case RoomType::MyRoom:
    {
        MyRoomRooms.erase(static_pointer_cast<MyRoomRoom>(room)->ownerId);
        break;
    }
    case RoomType::Meeting:
    {
        MeetingRooms.erase(static_pointer_cast<MeetingRoom>(room)->roomCode);
        break;
    }
    case RoomType::Lecture:
    {
        LectureRooms.erase(static_pointer_cast<OfficeRoom>(room)->roomCode);
        break;
    }
    case RoomType::Consulting:
    {
        ConsultingRooms.erase(static_pointer_cast<MeetingRoom>(room)->roomCode);
        break;
    }
    case RoomType::JumpingMatching:
    {
        JumpingMatchingRooms.erase(static_pointer_cast<MatchingRoom>(room)->roomCode);
        break;
    }
    case RoomType::OXQuiz:
    {
        OXQuizRooms.erase(static_pointer_cast<OXRoom>(room)->roomCode);
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
    case RoomType::Arz:
    {
        for (auto arzLandRoom = ArzRooms.begin(); arzLandRoom != ArzRooms.end(); arzLandRoom++)
            res.push_back(arzLandRoom->second->roomInfo);
        break;
    }
    case RoomType::Game:
    {
        for (auto gameZoneRoom = GameRooms.begin(); gameZoneRoom != GameRooms.end(); gameZoneRoom++)
            res.push_back(gameZoneRoom->second->roomInfo);
        break;
    }
    case RoomType::Conference:
    {
        for (auto ctfZoneRoom = ConferenceRooms.begin(); ctfZoneRoom != ConferenceRooms.end(); ctfZoneRoom++)
            res.push_back(ctfZoneRoom->second->roomInfo);
        break;
    }
    case RoomType::Vote:
    {
        for (auto voteZoneRoom = VoteRooms.begin(); voteZoneRoom != VoteRooms.end(); voteZoneRoom++)
            res.push_back(voteZoneRoom->second->roomInfo);
        break;
    }
    case RoomType::Store:
    {
        for (auto storeZoneRoom = StoreRooms.begin(); storeZoneRoom != StoreRooms.end(); storeZoneRoom++)
            res.push_back(storeZoneRoom->second->roomInfo);
        break;
    }
    case RoomType::Office:
    {
        for (auto officeLobbyZoneRoom = OfficeRooms.begin(); officeLobbyZoneRoom != OfficeRooms.end(); officeLobbyZoneRoom++)
            res.push_back(officeLobbyZoneRoom->second->roomInfo);
        break;
    }
    case RoomType::Busan:
    {
        for (auto busanLandRoom = BusanRooms.begin(); busanLandRoom != BusanRooms.end(); busanLandRoom++)
            res.push_back(busanLandRoom->second->roomInfo);
        break;
    }
    case RoomType::MyRoom:
    {
        if (query.find("ownerId") != query.end())
        {
            auto myroom = MyRoomRooms.find(query["ownerId"]);
            if (myroom != MyRoomRooms.end())
                res.push_back(myroom->second->roomInfo);
        }
        else
            for (auto myRoomRoom = MyRoomRooms.begin(); myRoomRoom != MyRoomRooms.end(); myRoomRoom++)
                res.push_back(myRoomRoom->second->roomInfo);
        break;
    }
    case RoomType::Meeting:
    {
        if (query.find("roomCode") != query.end())
        {
            auto meetingRoom = MeetingRooms.find(query["roomCode"]);
            if (meetingRoom != MeetingRooms.end())
                res.push_back(meetingRoom->second->roomInfo);
        }
        else
            for (auto meetingRoom = MeetingRooms.begin(); meetingRoom != MeetingRooms.end(); meetingRoom++)
                res.push_back(meetingRoom->second->roomInfo);
        break;
    }
    case RoomType::Lecture:
    {
        if (query.find("roomCode") != query.end())
        {
            auto lectureRoom = LectureRooms.find(query["roomCode"]);
            if (lectureRoom != LectureRooms.end())
                res.push_back(lectureRoom->second->roomInfo);
        }
        else
            for (auto lectureRoom = LectureRooms.begin(); lectureRoom != LectureRooms.end(); lectureRoom++)
                res.push_back(lectureRoom->second->roomInfo);
        break;
    }
    case RoomType::Consulting:
    {
        if (query.find("roomCode") != query.end())
        {
            auto counselRoom = ConsultingRooms.find(query["roomCode"]);
            if (counselRoom != ConsultingRooms.end())
                res.push_back(counselRoom->second->roomInfo);
        }
        else
            for (auto counselRoom = ConsultingRooms.begin(); counselRoom != ConsultingRooms.end(); counselRoom++)
                res.push_back(counselRoom->second->roomInfo);
        break;
    }
    case RoomType::JumpingMatching:
    {
        if (query.find("roomCode") != query.end())
        {
            auto jumpingMatchingRoom = JumpingMatchingRooms.find(query["roomCode"]);
            if (jumpingMatchingRoom != JumpingMatchingRooms.end())
                res.push_back(jumpingMatchingRoom->second->roomInfo);
        }
        else
            for (auto matchingRoom = JumpingMatchingRooms.begin(); matchingRoom != JumpingMatchingRooms.end(); matchingRoom++)
                res.push_back(matchingRoom->second->roomInfo);
        break;
    }
    case RoomType::OXQuiz:
    {
        if (query.find("roomCode") != query.end())
        {
            auto oxRoom = OXQuizRooms.find(query["roomCode"]);
            if (oxRoom != OXQuizRooms.end())
                res.push_back(oxRoom->second->roomInfo);
        }
        else
            for (auto oxRoom = OXQuizRooms.begin(); oxRoom != OXQuizRooms.end(); oxRoom++)
                res.push_back(oxRoom->second->roomInfo);
        break;
    }
    default:
        throw std::invalid_argument("Invalid RoomType value");
    }

    return res;
}
