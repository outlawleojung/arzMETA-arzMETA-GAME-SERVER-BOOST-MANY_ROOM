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
        
        {"Festival", RoomType::Festival},

        {"Unknown", RoomType::Unknown}
    };

    auto it = stringToRoomType.find(roomTypeStr);
    if (it != stringToRoomType.end()) {
        return it->second;
    }

    return RoomType::Unknown;
}

std::string roomTypeToString(RoomType roomType) {
    static std::map<RoomType, std::string> roomTypeToString = {
        {RoomType::Arz, "Arz"},
        {RoomType::Busan, "Busan"},

        {RoomType::JumpingMatching, "JumpingMatching"},
        {RoomType::OXQuiz, "OXQuiz"},

        {RoomType::Lecture, "Lecture"},
        {RoomType::Meeting, "Meeting"},
        {RoomType::Consulting, "Consulting"},

        {RoomType::MyRoom, "MyRoom"},

        {RoomType::Conference, "Conference"},
        {RoomType::Game, "Game"},
        {RoomType::Office, "Office"},
        {RoomType::Store, "Store"},
        {RoomType::Vote, "Vote"},

        {RoomType::Hospital, "Hospital"},

        {RoomType::Festival, "Festival"},

        {RoomType::Unknown, "Unknown"}
    };

    auto it = roomTypeToString.find(roomType);
    if (it != roomTypeToString.end()) {
        return it->second;
    }

    return "Unknown";
}
