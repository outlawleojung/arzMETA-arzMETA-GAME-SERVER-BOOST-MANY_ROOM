#include "RoomTypes.h"

RoomType stringToRoomType(const std::string& roomTypeStr) {
    static std::map<std::string, RoomType> stringToRoomType = {
        {"ArzLand", RoomType::ArzLand},
        {"GameZone", RoomType::GameZone},
        {"CTFZone", RoomType::CTFZone},
        {"VoteZone", RoomType::VoteZone},
        {"StoreZone", RoomType::StoreZone},
        {"OfficeLobbyZone", RoomType::OfficeLobbyZone},
        {"BusanLand", RoomType::BusanLand},
        {"MyRoom", RoomType::MyRoom},
        {"Office", RoomType::Office},
        {"Meeting", RoomType::Meeting},
        {"Lecture", RoomType::Lecture},
        {"Counsel", RoomType::Counsel},
        {"JumpingMatching", RoomType::JumpingMatching},
        {"OX", RoomType::OX},
        {"Unknown", RoomType::Unknown}
    };

    auto it = stringToRoomType.find(roomTypeStr);
    if (it != stringToRoomType.end()) {
        return it->second;
    }

    return RoomType::Unknown;
}