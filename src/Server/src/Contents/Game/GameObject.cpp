#include "GameObject.h"

void GameObject::MakeGameObjectInfo(Protocol::S_BASE_ADD_OBJECT_GameObjectInfo* gameObjectInfo)
{
	gameObjectInfo->set_objectid(objectId);
	gameObjectInfo->set_prefabname(prefabName);
	gameObjectInfo->set_objectdata(objectData);
	auto _position = gameObjectInfo->mutable_position();
	_position->set_x(position[0]);
	_position->set_y(position[1]);
	_position->set_z(position[2]);
	auto _rotation = gameObjectInfo->mutable_rotation();
	_rotation->set_x(rotation[0]);
	_rotation->set_y(rotation[1]);
	_rotation->set_z(rotation[2]);
	gameObjectInfo->set_ownerid(ownerId);
	gameObjectInfo->set_animationid(animationId);
	gameObjectInfo->set_isloop(isLoop);
	gameObjectInfo->set_blend(blend);
}

void GameObject::MakeTransform(Protocol::S_BASE_SET_TRANSFORM& transform)
{
	transform.set_objectid(objectId);

	auto _position = transform.mutable_position();
	_position->set_x(position[0]);
	_position->set_y(position[1]);
	_position->set_z(position[2]);

	auto _rotation = transform.mutable_rotation();
	_rotation->set_x(rotation[0]);
	_rotation->set_y(rotation[1]);
	_rotation->set_z(rotation[2]);
}

void GameObject::SetPosition(float x, float y, float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

void GameObject::SetRotation(float x, float y, float z)
{
	rotation[0] = x;
	rotation[1] = y;
	rotation[2] = z;
}
