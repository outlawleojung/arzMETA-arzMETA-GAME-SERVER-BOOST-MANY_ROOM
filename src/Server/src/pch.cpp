#include "pch.h"

#include "Contents/RoomManager.h"
#include "Contents/ClientManager.h"

RoomManager* GRoomManager = new RoomManager;
ClientManager* GClientManager = new ClientManager;

string localHostIp;
int tcpPort = 7777;
int httpPort = 8080;

string DBDomain = std::getenv("DB_DOMAIN") != nullptr ? string(std::getenv("DB_DOMAIN")) : "dev-arowana-mysql.mariadb.database.azure.com";
string DBUsername = std::getenv("DB_USERNAME") != nullptr ? string(std::getenv("DB_USERNAME")) : "frontishub@dev-arowana-mysql";
string DBPassword = std::getenv("DB_PASSWORD") != nullptr ? string(std::getenv("DB_PASSWORD")) : "rkskekfk123!@!@";