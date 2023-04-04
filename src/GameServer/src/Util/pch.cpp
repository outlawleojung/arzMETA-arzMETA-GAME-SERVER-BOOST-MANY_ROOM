#include "pch.h"
#include "../GameContents/RoomManager.h"
#include "../GameContents/ClientManager.h"

RoomManager* GRoomManager = new RoomManager;
ClientManager* GClientManager = new ClientManager;

string localHostIp;
int tcpPort = 7777;
int httpPort = 8080;