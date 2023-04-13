#pragma once

#include "../Game/GameClient.h"

class OXClient : public GameClient
{
public :
	std::chrono::system_clock::time_point enteredTime;
};