#include "pch.h"

#include "Contents/RoomManager.h"
#include "Contents/ClientManager.h"

RoomManager* GRoomManager = new RoomManager;
ClientManager* GClientManager = new ClientManager;

string localHostIp;
int tcpPort = 45456;
int httpPort = 45457;