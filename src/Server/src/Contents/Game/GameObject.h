#pragma once

#include "../../pch.h"
#include "../../Protocols.h"

class GameObject
{
public:
	GameObject(int objectId)
		: objectId(objectId)
	{
		position.resize(3);
		rotation.resize(3);

		GLogManager->Log("GameObject Created :			", to_string(objectId));
	};
	~GameObject()
	{
		GLogManager->Log("GameObject Destroyed :		", to_string(objectId));
	}

public:
	void MakeGameObjectInfo(Protocol::S_BASE_ADD_OBJECT_GameObjectInfo* gameObjectInfo);
	void MakeTransform(Protocol::S_BASE_SET_TRANSFORM& transform);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	int objectId;
	std::vector<float> position;
	std::vector<float> rotation;
	std::map<string, string> animations;
	string prefabName;
	string objectData;

	string ownerId;
};

