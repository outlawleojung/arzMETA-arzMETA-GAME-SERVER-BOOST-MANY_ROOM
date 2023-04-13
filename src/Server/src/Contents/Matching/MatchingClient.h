#pragma once

#include "../Game/GameClient.h"

class MatchingClient : public GameClient
{
public :
	std::chrono::system_clock::time_point enteredTime;
};