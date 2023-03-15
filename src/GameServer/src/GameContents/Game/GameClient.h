#pragma once

#include "../Base/ClientBase.h"

class Scene;

class GameClient : public ClientBase
{
public :
	shared_ptr<Scene> scene = nullptr;
	vector<int> gameObjectIds;
};