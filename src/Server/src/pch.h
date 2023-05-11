#pragma once

#include <pch.h>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

extern class RoomManager* GRoomManager;
extern class ClientManager* GClientManager;

extern string localHostIp;
extern int tcpPort;
extern int httpPort;

extern string DBDomain;
extern string DBUsername;
extern string DBPassword;
extern string DBSchema;