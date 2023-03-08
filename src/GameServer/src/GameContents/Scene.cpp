#include "Scene.h"
#include "GameObject.h"

#include "../ClientPacketHandler.h"

#include "../Session/GameSession.h"

Scene::Scene(string id)
	: id(id)
{}

Scene::~Scene()
{}

void Scene::Clear()
{
	for (auto client = sessions.begin(); client != sessions.end(); client++)
	{
		client->second->gameObjectIds.clear();
		client->second->scene = nullptr;
	}

	gameObjects.clear();
	sessions.clear();
}

void Scene::Leave(shared_ptr<GameSession> session)
{
	Protocol::S_BASE_REMOVE_OBJECT removeObject;
	for (auto gameObjectId = session->gameObjectIds.begin(); gameObjectId != session->gameObjectIds.end(); gameObjectId++)
	{
		auto scene_gameObject = gameObjects.find(*gameObjectId);
		//if (scene_gameObject == gameObjects.end())
		//	continue;
		gameObjects.erase(scene_gameObject);
		removeObject.add_gameobjects(*gameObjectId);
	}
	session->gameObjectIds.clear();

	sessions.erase(sessions.find(session->clientId));

	//auto sessionEntry = sessions.find(session->clientId);
	//if(sessionEntry != sessions.end())
	//	sessions.erase(sessionEntry);

	session->scene = nullptr;

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(removeObject);
	for (auto _session = sessions.begin(); _session != sessions.end(); _session++)
		_session->second->Send(sendBuffer);
}

void Scene::AddSession(shared_ptr<GameSession> session)
{
	sessions.insert({ session->clientId, session });
}

void Scene::InstantiateObject(shared_ptr<GameSession> session, Protocol::C_BASE_INSTANTIATE_OBJECT pkt)
{
	auto gameObject = make_shared<GameObject>(session->scene->idGenerator++);
	gameObject->prefabName = pkt.prefabname();
	gameObject->objectData = pkt.objectdata();
	gameObject->SetPosition(pkt.position().x(), pkt.position().y(), pkt.position().z());
	gameObject->SetRotation(pkt.rotation().x(), pkt.rotation().y(), pkt.rotation().z());
	gameObject->ownerId = session->clientId;
	
	gameObjects.insert({ gameObject->objectId, gameObject });
	session->gameObjectIds.push_back(gameObject->objectId);

	{
		Protocol::S_BASE_INSTANTIATE_OBJECT res;
		res.set_success(true);
		res.set_objectid(gameObject->objectId);
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
	}

	{
		Protocol::S_BASE_ADD_OBJECT instantiateObjectNotice;
		auto gameObjectPacket = instantiateObjectNotice.add_gameobjects();
		gameObject->MakeGameObjectInfo(gameObjectPacket);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(instantiateObjectNotice);
		Broadcast(sendBuffer);
	}
}

void Scene::GetObjects(shared_ptr<GameSession> session)
{
	Protocol::S_BASE_ADD_OBJECT res;

	for (auto gameObject = gameObjects.begin(); gameObject != gameObjects.end(); gameObject++)
	{
		auto gameObjectPacket = res.add_gameobjects();
		gameObject->second->MakeGameObjectInfo(gameObjectPacket);
	}

	if (res.gameobjects_size() > 0)
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
}

void Scene::SetTransfrom(shared_ptr<GameSession> session, int objectId, float position_x, float position_y, float position_z, float rotation_x, float rotation_y, float rotation_z)
{
	auto gameObject = gameObjects.find(objectId);
	if (gameObject == gameObjects.end())
		return;

	gameObject->second->SetPosition(position_x, position_y, position_z);
	gameObject->second->SetRotation(rotation_x, rotation_y, rotation_z);

	Protocol::S_BASE_SET_TRANSFORM res;
	gameObject->second->MakeTransform(res);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(res);
	Broadcast(sendBuffer);
}

void Scene::SetAnimation(shared_ptr<GameSession> session, int objectId, string animationId, string animationValue)
{
	auto gameObject = gameObjects.find(objectId);
	if (gameObject == gameObjects.end())
		return;

	auto animation = gameObject->second->animations.find(animationId);
	if (animation == gameObject->second->animations.end())
		gameObject->second->animations.insert({ animationId, animationValue });
	else
		animation->second = animationValue;

	Protocol::S_BASE_SET_ANIMATION res;
	res.set_objectid(objectId);
	res.set_animationid(animationId);
	res.set_animation(animationValue);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(res);
	Broadcast(sendBuffer);
}

void Scene::SetAnimationOnce(shared_ptr<GameSession> session, int objectId, string animationId, bool isLoop, float blend)
{
	auto gameObject = gameObjects.find(objectId);
	if (gameObject == gameObjects.end())
		return;

	Protocol::S_BASE_SET_ANIMATION_ONCE res;
	res.set_objectid(objectId);
	res.set_animationid(animationId);
	res.set_isloop(isLoop);
	res.set_blend(blend);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(res);
	Broadcast(sendBuffer);
}

void Scene::GetStates(shared_ptr<GameSession> session)
{
	Protocol::S_INTERACTION_GET_ITEMS res;

	for (auto state = states.begin(); state != states.end(); state++)
	{
		auto item = res.add_items();
		item->set_id(state->first);
		item->set_state(state->second);
	}

	session->Send(ClientPacketHandler::MakeSendBuffer(res));
}

void Scene::SetState(shared_ptr<GameSession> session, string id, string value)
{
	Protocol::S_INTERACTION_SET_ITEM res;

	auto state = states.find(id);
	if (state != states.end())
	{
		if (state->second == value)
		{
			res.set_success(false);
			session->Send(ClientPacketHandler::MakeSendBuffer(res));
			return;
		}

		state->second = value;
	}
	else
	{
		states.insert({ id, value });
	}

	res.set_success(true);
	session->Send(ClientPacketHandler::MakeSendBuffer(res));

	Protocol::S_INTERACTION_SET_ITEM_NOTICE notice;
	notice.set_id(id);
	notice.set_state(value);
	Broadcast(ClientPacketHandler::MakeSendBuffer(notice));
}

void Scene::RemoveState(shared_ptr<GameSession> session, string id)
{
	Protocol::S_INTERACTION_REMOVE_ITEM res;

	auto state = states.find(id);
	if (state == states.end())
	{
		res.set_success(false);
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		return;
	}

	states.erase(state);

	res.set_success(true);
	session->Send(ClientPacketHandler::MakeSendBuffer(res));

	Protocol::S_INTERACTION_REMOVE_ITEM_NOTICE notice;
	notice.set_id(id);
	Broadcast(ClientPacketHandler::MakeSendBuffer(notice));
}

void Scene::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	for (auto client = sessions.begin(); client != sessions.end(); client++)
		client->second->Send(sendBuffer);
}
