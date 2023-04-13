#pragma once

#include <map>
#include <string>

enum class RoomType
{
	ArzLand,
	GameZone,
	CTFZone,
	VoteZone,
	BusanLand,
	MyRoom,
	Office,
	Meeting,
	Lecture,
	Counsel,
	JumpingMatching,
	OX,
    Unknown
};

RoomType stringToRoomType(const std::string& roomTypeStr);