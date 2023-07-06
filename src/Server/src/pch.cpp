#include "pch.h"

#include "Contents/RoomManager.h"
#include "Contents/ClientManager.h"

RoomManager* GRoomManager = new RoomManager;
ClientManager* GClientManager = new ClientManager;

#include <soci/soci.h>
soci::connection_pool* DBConnectionPool = new soci::connection_pool(5);

string localHostIp;
int tcpPort = 7777;
int httpPort = 8080;

string DBDomain = std::getenv("DB_DOMAIN") != nullptr ? string(std::getenv("DB_DOMAIN")) : "dev-arowana-mysql.mariadb.database.azure.com";
string DBUsername = std::getenv("DB_USERNAME") != nullptr ? string(std::getenv("DB_USERNAME")) : "frontishub@dev-arowana-mysql";
string DBPassword = std::getenv("DB_PASSWORD") != nullptr ? string(std::getenv("DB_PASSWORD")) : "rkskekfk!!!123";
string DBSchema = std::getenv("DB_SCHEMA") != nullptr ? string(std::getenv("DB_SCHEMA")) : "dev_arzmeta_db";