#pragma once

#include <map>
#include <string>

enum class RoomType
{
    Arz,
    Busan,

    JumpingMatching,
    OXQuiz,

    Lecture,
    Meeting,
    Consulting,

    MyRoom,

    Conference,
    Game,
    Office,
    Store,
    Vote,

    Hospital,

    Festival,

    Unknown
};

RoomType stringToRoomType(const std::string& roomTypeStr);
std::string roomTypeToString(RoomType roomType);