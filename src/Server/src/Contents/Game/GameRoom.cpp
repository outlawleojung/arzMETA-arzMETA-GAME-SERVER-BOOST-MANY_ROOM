#include "GameRoom.h"

#include "GameClient.h"
#include "GameObject.h"
#include "../ClientManager.h"
#include "../RoomManager.h"
#include "../../PacketManager.h"
#include "../../Session/GameSession.h"
#include "../RoomTypes.h"

GameRoom::GameRoom()
{
	maxPlayerNumber = 10;
}

void GameRoom::Init()
{
	state = RoomState::Running;

	roomInfo["roomId"] = roomId;
	roomInfo["sceneName"] = sceneName;

	roomInfo["roomType"] = roomTypeToString(type);

	GRoomManager->IndexRoom(static_pointer_cast<RoomBase>(shared_from_this()));

	this->DoTimer(30000, std::function<void()>(
		[this]() {
			if (this->state != RoomState::Running)
				return;

			if (this->clients.size() == 0)
				Close();
		}
	));
}

void GameRoom::Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) { DoAsync(&GameRoom::Enter, session, pkt); }
void GameRoom::Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<ClientBase>& client, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt)
{ 
	auto gClient = static_pointer_cast<GameClient>(client);
	DoAsync(&GameRoom::InstantiateObject, gClient, pkt);
}
void GameRoom::Handle_C_BASE_REMOVE_OBJECT(shared_ptr<ClientBase>& client, Protocol::C_BASE_REMOVE_OBJECT& pkt)
{
	auto gClient = static_pointer_cast<GameClient>(client);
	DoAsync(&GameRoom::RemoveObject, gClient);
}
void GameRoom::Handle_C_BASE_GET_OBJECT(shared_ptr<ClientBase>& client, Protocol::C_BASE_GET_OBJECT& pkt)
{
	auto gClient = static_pointer_cast<GameClient>(client);
	DoAsync(&GameRoom::GetObjects, gClient);
}
void GameRoom::Handle_C_BASE_SET_OBJECT_DATA(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_OBJECT_DATA& pkt)
{
	auto gClient = static_pointer_cast<GameClient>(client);
	DoAsync(&GameRoom::SetObjectData, gClient, pkt.objectid(), pkt.objectdata());
}
void GameRoom::Handle_C_BASE_SET_TRANSFORM(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_TRANSFORM& pkt)
{ 
	auto gClient = static_pointer_cast<GameClient>(client);
	DoAsync(&GameRoom::SetTransfrom, pkt);
}
void GameRoom::Handle_C_BASE_SET_ANIMATION(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_ANIMATION& pkt) { DoAsync(&GameRoom::SetAnimation, pkt.objectid(), pkt.animationid(), pkt.animation()); }
void GameRoom::Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt) { 	DoAsync(&GameRoom::SetAnimationOnce, pkt.objectid(), pkt.animationid(), pkt.isloop(), pkt.blend()); }

void GameRoom::Leave(shared_ptr<ClientBase> client)
{
	if (state != RoomState::Running) return;

	auto gClient = static_pointer_cast<GameClient>(client);

	RemoveObject(gClient);

	RoomBase::Leave(client);
}

shared_ptr<ClientBase> GameRoom::MakeClient(string clientId, int sessionId)
{
	return GClientManager->MakeCilent<GameClient>(clientId, sessionId, static_pointer_cast<RoomBase>(shared_from_this()));
}

pair<bool, string> GameRoom::HandleEnter(const Protocol::C_ENTER& pkt)
{
	if (clients.size() >= maxPlayerNumber)
		return { false, "ROOM_IS_FULL" };

	return { true, "SUCCESS" };
}

void GameRoom::RemoveObject(shared_ptr<GameClient> client)
{
	if (state != RoomState::Running) return;

	Protocol::S_BASE_REMOVE_OBJECT removeObject;

	for (auto gameObjectId : client->gameObjects)
		if (gameObjects.erase(gameObjectId))
			removeObject.add_gameobjects(gameObjectId);

	client->gameObjects.clear();

	if (removeObject.gameobjects_size() <= 0)
		return;

	Broadcast(PacketManager::MakeSendBuffer(removeObject));
}

void GameRoom::InstantiateObject(shared_ptr<GameClient> client, Protocol::C_BASE_INSTANTIATE_OBJECT pkt)
{
	if (state != RoomState::Running) return;

	auto gameObject = make_shared<GameObject>(idGenerator++);
	gameObject->prefabName = pkt.prefabname();
	gameObject->objectData = pkt.objectdata();
	gameObject->SetPosition(pkt.position().x(), pkt.position().y(), pkt.position().z());
	gameObject->SetRotation(pkt.rotation().x(), pkt.rotation().y(), pkt.rotation().z());
	gameObject->ownerId = client->clientId;

	gameObjects.insert({ gameObject->objectId, gameObject });

	client->gameObjects.insert(gameObject->objectId);

	{
		Protocol::S_BASE_INSTANTIATE_OBJECT res;
		res.set_success(true);
		res.set_objectid(gameObject->objectId);
		client->Send(PacketManager::MakeSendBuffer(res));
	}

	{
		Protocol::S_BASE_ADD_OBJECT instantiateObjectNotice;
		auto gameObjectPacket = instantiateObjectNotice.add_gameobjects();
		gameObject->MakeGameObjectInfo(gameObjectPacket);
		auto sendBuffer = PacketManager::MakeSendBuffer(instantiateObjectNotice);
		Broadcast(sendBuffer);
	}
}

void GameRoom::GetObjects(shared_ptr<GameClient> client)
{
	if (state != RoomState::Running) return;

	Protocol::S_BASE_ADD_OBJECT res;

	for (auto gameObject = gameObjects.begin(); gameObject != gameObjects.end(); gameObject++)
	{
		auto gameObjectPacket = res.add_gameobjects();
		gameObject->second->MakeGameObjectInfo(gameObjectPacket);
	}

	if (res.gameobjects_size() > 0)
		client->Send(PacketManager::MakeSendBuffer(res));
}

void GameRoom::SetObjectData(shared_ptr<GameClient> client, int objectId, string objectData)
{
	Protocol::S_BASE_SET_OBJECT_DATA res;

	if (!client->gameObjects.count(objectId) || !gameObjects.count(objectId))
	{
		res.set_success(false);
		client->Send(PacketManager::MakeSendBuffer(res));
		return;
	}

	res.set_success(true);
	client->Send(PacketManager::MakeSendBuffer(res));

	auto gameObject = gameObjects[objectId];
	gameObject->objectData = objectData;

	Protocol::S_BASE_SET_OBJECT_DATA_NOTICE setObjectDataNotice;
	setObjectDataNotice.set_objectid(objectId);
	setObjectDataNotice.set_objectdata(objectData);
	auto sendBuffer = PacketManager::MakeSendBuffer(setObjectDataNotice);
	Broadcast(sendBuffer);
}

void GameRoom::SetTransfrom(Protocol::C_BASE_SET_TRANSFORM pkt)
{
	if (state != RoomState::Running) return;

	auto gameObject = gameObjects.find(pkt.objectid());
	if (gameObject == gameObjects.end())
		return;

	gameObject->second->SetPosition(pkt.position().x(), pkt.position().y(), pkt.position().z());
	gameObject->second->SetRotation(pkt.rotation().x(), pkt.rotation().y(), pkt.rotation().z());

	Protocol::S_BASE_SET_TRANSFORM res;
	res.set_objectid(pkt.objectid());
	res.set_allocated_position(pkt.release_position());
	res.set_allocated_rotation(pkt.release_rotation());
	auto sendBuffer = PacketManager::MakeSendBuffer(res);
	Broadcast(sendBuffer);
}

void GameRoom::SetAnimation(int objectId, string animationId, string animationValue)
{
	if (state != RoomState::Running) return;

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
	auto sendBuffer = PacketManager::MakeSendBuffer(res);
	Broadcast(sendBuffer);
}

void GameRoom::SetAnimationOnce(int objectId, string animationId, bool isLoop, float blend)
{
	if (state != RoomState::Running) return;

	auto gameObject = gameObjects.find(objectId);
	if (gameObject == gameObjects.end())
		return;

	Protocol::S_BASE_SET_ANIMATION_ONCE res;
	res.set_objectid(objectId);
	res.set_animationid(animationId);
	res.set_isloop(isLoop);
	res.set_blend(blend);
	auto sendBuffer = PacketManager::MakeSendBuffer(res);
	Broadcast(sendBuffer);
}

void GameRoom::ClearObject()
{
	Protocol::S_BASE_REMOVE_OBJECT removeObject;

	for (auto gameObject = gameObjects.begin(); gameObject != gameObjects.end(); gameObject++)
		removeObject.add_gameobjects(gameObject->second->objectId);

	Broadcast(PacketManager::MakeSendBuffer(removeObject));
	gameObjects.clear();
	for (auto client = clients.begin(); client != clients.end(); client++)
	{
		auto gClient = static_pointer_cast<GameClient>(client->second);
		gClient->gameObjects.clear();
	}
}

