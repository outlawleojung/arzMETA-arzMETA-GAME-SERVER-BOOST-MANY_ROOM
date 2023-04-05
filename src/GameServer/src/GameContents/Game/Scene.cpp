#include "Scene.h"
#include "GameObject.h"
#include "GameClient.h"
#include "../../ClientPacketHandler.h"

Scene::Scene(string id)
	: id(id)
{}

Scene::~Scene()
{}

void Scene::Clear()
{
	for (auto client = clients.begin(); client != clients.end(); client++)
		client->second->scene = nullptr;

	gameObjects.clear();
	clients.clear();
}

void Scene::Enter(shared_ptr<GameClient> client)
{
	client->scene = static_pointer_cast<Scene>(shared_from_this());
	clients.insert({ client->clientId, client });

	Protocol::S_BASE_SET_SCENE res;
	res.set_success(true);
	client->Send(ClientPacketHandler::MakeSendBuffer(res));
}

void Scene::Leave(shared_ptr<GameClient> client)
{
	clients.erase(clients.find(client->clientId));
	RemoveObject(client);
}

void Scene::InstantiateObject(shared_ptr<GameClient> client, Protocol::C_BASE_INSTANTIATE_OBJECT pkt)
{
	auto gameObject = make_shared<GameObject>(idGenerator++);
	gameObject->prefabName = pkt.prefabname();
	gameObject->objectData = pkt.objectdata();
	gameObject->SetPosition(pkt.position().x(), pkt.position().y(), pkt.position().z());
	gameObject->SetRotation(pkt.rotation().x(), pkt.rotation().y(), pkt.rotation().z());
	gameObject->ownerId = client->clientId;
	
	gameObjects.insert({ gameObject->objectId, gameObject });
	
	if (client_gameObjects.find(client->clientId) == client_gameObjects.end())
		client_gameObjects.insert({ client->clientId, vector<int>() });
	client_gameObjects.find(client->clientId)->second.push_back(gameObject->objectId);

	{
		Protocol::S_BASE_INSTANTIATE_OBJECT res;
		res.set_success(true);
		res.set_objectid(gameObject->objectId);
		client->Send(ClientPacketHandler::MakeSendBuffer(res));
	}

	{
		Protocol::S_BASE_ADD_OBJECT instantiateObjectNotice;
		auto gameObjectPacket = instantiateObjectNotice.add_gameobjects();
		gameObject->MakeGameObjectInfo(gameObjectPacket);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(instantiateObjectNotice);
		Broadcast(sendBuffer);
	}
}

void Scene::RemoveObject(shared_ptr<GameClient> client)
{
	Protocol::S_BASE_REMOVE_OBJECT removeObject;

	auto client_gameObject = client_gameObjects.find(client->clientId);
	if (client_gameObject != client_gameObjects.end())
	{
		for (auto gameObjectId = client_gameObject->second.begin(); gameObjectId != client_gameObject->second.end(); gameObjectId++)
		{
			auto scene_gameObject = gameObjects.find(*gameObjectId);
			gameObjects.erase(scene_gameObject);
			removeObject.add_gameobjects(*gameObjectId);
		}

		client_gameObjects.erase(client_gameObject);
	}

	if (removeObject.gameobjects().size() <= 0)
		return;

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(removeObject);
	for (auto _session = clients.begin(); _session != clients.end(); _session++)
		_session->second->Send(sendBuffer);
}

void Scene::GetObjects(shared_ptr<GameClient> client)
{
	Protocol::S_BASE_ADD_OBJECT res;

	for (auto gameObject = gameObjects.begin(); gameObject != gameObjects.end(); gameObject++)
	{
		auto gameObjectPacket = res.add_gameobjects();
		gameObject->second->MakeGameObjectInfo(gameObjectPacket);
	}

	if (res.gameobjects_size() > 0)
		client->Send(ClientPacketHandler::MakeSendBuffer(res));
}

void Scene::SetTransfrom(int objectId, float position_x, float position_y, float position_z, float rotation_x, float rotation_y, float rotation_z)
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

void Scene::SetAnimation(int objectId, string animationId, string animationValue)
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

void Scene::SetAnimationOnce(int objectId, string animationId, bool isLoop, float blend)
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

void Scene::GetStates(shared_ptr<GameClient> client)
{
	Protocol::S_INTERACTION_GET_ITEMS res;

	for (auto state = states.begin(); state != states.end(); state++)
	{
		auto item = res.add_items();
		item->set_id(state->first);
		item->set_state(state->second);
	}

	client->Send(ClientPacketHandler::MakeSendBuffer(res));
}

void Scene::SetState(shared_ptr<GameClient> client, string id, string value)
{
	Protocol::S_INTERACTION_SET_ITEM res;

	auto state = states.find(id);
	if (state != states.end())
	{
		if (state->second == value)
		{
			res.set_success(false);
			client->Send(ClientPacketHandler::MakeSendBuffer(res));
			return;
		}

		state->second = value;
	}
	else
	{
		states.insert({ id, value });
	}

	res.set_success(true);
	client->Send(ClientPacketHandler::MakeSendBuffer(res));

	Protocol::S_INTERACTION_SET_ITEM_NOTICE notice;
	notice.set_id(id);
	notice.set_state(value);
	Broadcast(ClientPacketHandler::MakeSendBuffer(notice));
}

void Scene::RemoveState(shared_ptr<GameClient> client, string id)
{
	Protocol::S_INTERACTION_REMOVE_ITEM res;

	auto state = states.find(id);
	if (state == states.end())
	{
		res.set_success(false);
		client->Send(ClientPacketHandler::MakeSendBuffer(res));
		return;
	}

	states.erase(state);

	res.set_success(true);
	client->Send(ClientPacketHandler::MakeSendBuffer(res));

	Protocol::S_INTERACTION_REMOVE_ITEM_NOTICE notice;
	notice.set_id(id);
	Broadcast(ClientPacketHandler::MakeSendBuffer(notice));
}

void Scene::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	for (auto client = clients.begin(); client != clients.end(); client++)
		client->second->Send(sendBuffer);
}
