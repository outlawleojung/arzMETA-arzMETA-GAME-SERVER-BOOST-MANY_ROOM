#include "RoomTypes.h"

RoomType stringToRoomType(const std::string& roomTypeStr) {
    static std::map<std::string, RoomType> stringToRoomType = {
        {"Arz", RoomType::Arz},
        {"Busan", RoomType::Busan},

        {"JumpingMatching", RoomType::JumpingMatching},
        {"OXQuiz", RoomType::OXQuiz},

        {"Lecture", RoomType::Lecture},
        {"Meeting", RoomType::Meeting},
        {"Consulting", RoomType::Consulting},

        {"MyRoom", RoomType::MyRoom},

        {"Conference", RoomType::Conference},
        {"Game", RoomType::Game},
        {"Office", RoomType::Office},
        {"Store", RoomType::Store},
        {"Vote", RoomType::Vote},

        {"Hospital", RoomType::Hospital},
         
        {"Unknown", RoomType::Unknown}
    };

    auto it = stringToRoomType.find(roomTypeStr);
    if (it != stringToRoomType.end()) {
        return it->second;
    }

    return RoomType::Unknown;
}
