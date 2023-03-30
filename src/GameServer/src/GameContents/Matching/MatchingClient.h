#pragma once

#include "../Game/GameClient.h"

class MatchingClient : public GameClient
{
public :
	chrono::system_clock::time_point enteredTime;
};