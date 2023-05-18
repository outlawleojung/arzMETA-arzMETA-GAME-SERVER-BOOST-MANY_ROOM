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

static string GetCurrentTimeString()
{
	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm bt;

	t += 32400;

#ifdef linux
	localtime_r(&t, &bt);
#elif _WIN32
	localtime_s(&bt, &t);
#endif
	char buffer[50];
	strftime(buffer, 50, "%Y.%m.%d %p %I:%M", &bt);
	return string(buffer);
}