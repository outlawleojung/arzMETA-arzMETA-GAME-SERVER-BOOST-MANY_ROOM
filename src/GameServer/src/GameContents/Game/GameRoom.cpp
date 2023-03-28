#include "GameRoom.h"

#include "../../ClientPacketHandler.h"
#include "../../Session/GameSession.h"
#include "../ClientManager.h"
#include "GameClient.h"
#include "Scene.h"

GameRoom::GameRoom(vector<string> sceneIds)
{
	for (auto sceneId = sceneIds.begin(); sceneId != sceneIds.end(); sceneId++)
		scenes.insert({ *sceneId, make_shared<Scene>(*sceneId) });

	maxPlayerNumber = 10;
}

void GameRoom::HandleClose()
{
	RoomBase::HandleClose();
}

void GameRoom::Clear()
{
	for (auto scene = scenes.begin(); scene != scenes.end(); scene++)
		scene->second->Clear();

	scenes.clear();
}

void GameRoom::Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt)
{
	Protocol::S_ENTER res;

	if (state != RoomState::Running)
	{
		session->Disconnect();
		return;
	}

	{
		auto client = clients.find(pkt.clientid());
		if (client != clients.end())
		{
			client->second->DoAsync(&ClientBase::Leave, string("DUPLICATED"));
			DoTimer(1000, &GameRoom::Enter, session, pkt);
			return;
		}
	}

	if (clients.size() >= maxPlayerNumber)
	{
		res.set_result("ROOM_IS_FULL");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		session->Disconnect();
		return;
	}

	auto client = static_pointer_cast<GameClient>(GClientManager->MakeCilent<GameClient>(session, pkt.clientid(), pkt.nickname(), static_pointer_cast<RoomBase>(shared_from_this())));

	clients.insert({ pkt.clientid(), client });

	res.set_result("SUCCESS");
	session->Send(ClientPacketHandler::MakeSendBuffer(res));
}

void GameRoom::Leave(shared_ptr<ClientBase> client)
{
	auto gClient = static_pointer_cast<GameClient>(client);

	if (gClient->scene != nullptr)
		//Scene 의 Leave 는 직접 호출할 수 없고 DoAsync 를 이용해야 함
		gClient->scene->DoAsync(&Scene::Leave, gClient);

	RoomBase::Leave(client);
}

void GameRoom::Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) { DoAsync(&GameRoom::Enter, session, pkt); }

void GameRoom::Handle_C_BASE_SET_SCENE(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_SCENE& pkt) { DoAsync(&GameRoom::SetScene, client, pkt.sceneid()); }
void GameRoom::Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<ClientBase>& client, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt)
{ 
	auto gClient = static_pointer_cast<GameClient>(client);

	if (gClient->scene == nullptr)
		return;
	gClient->scene->DoAsync(&Scene::InstantiateObject, gClient, pkt);
}
void GameRoom::Handle_C_BASE_GET_OBJECT(shared_ptr<ClientBase>& client, Protocol::C_BASE_GET_OBJECT& pkt)
{
	auto gClient = static_pointer_cast<GameClient>(client);

	if (gClient->scene == nullptr)
		return;
	gClient->scene->DoAsync(&Scene::GetObjects, gClient);
}
void GameRoom::Handle_C_BASE_SET_TRANSFORM(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_TRANSFORM& pkt)
{ 
	auto gClient = static_pointer_cast<GameClient>(client);

	if (gClient->scene == nullptr)
		return;
	gClient->scene->DoAsync(&Scene::SetTransfrom, pkt.objectid(), pkt.position().x(), pkt.position().y(), pkt.position().z(), pkt.rotation().x(), pkt.rotation().y(), pkt.rotation().z());
}
void GameRoom::Handle_C_BASE_SET_ANIMATION(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_ANIMATION& pkt)
{
	auto gClient = static_pointer_cast<GameClient>(client);

	if (gClient->scene == nullptr)
		return;
	gClient->scene->DoAsync(&Scene::SetAnimation, pkt.objectid(), pkt.animationid(), pkt.animation());
}
void GameRoom::Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt)
{
	auto gClient = static_pointer_cast<GameClient>(client);

	if (gClient->scene == nullptr)
		return;
	gClient->scene->DoAsync(&Scene::SetAnimationOnce, pkt.objectid(), pkt.animationid(), pkt.isloop(), pkt.blend());
}

void GameRoom::Handle_C_INTERACTION_GET_ITEMS(shared_ptr<ClientBase>& client, Protocol::C_INTERACTION_GET_ITEMS& pkt)
{
	auto gClient = static_pointer_cast<GameClient>(client);

	if (gClient->scene == nullptr)
		return;
	gClient->scene->DoAsync(&Scene::GetStates, gClient);
}

void GameRoom::Handle_C_INTERACTION_SET_ITEM(shared_ptr<ClientBase>& client, Protocol::C_INTERACTION_SET_ITEM& pkt)
{
	auto gClient = static_pointer_cast<GameClient>(client);

	if (gClient->scene == nullptr)
		return;
	gClient->scene->DoAsync(&Scene::SetState, gClient, pkt.id(), pkt.state());
}

void GameRoom::Handle_C_INTERACTION_REMOVE_ITEM(shared_ptr<ClientBase>& client, Protocol::C_INTERACTION_REMOVE_ITEM& pkt)
{
	auto gClient = static_pointer_cast<GameClient>(client);

	if (gClient->scene == nullptr)
		return;
	gClient->scene->DoAsync(&Scene::RemoveState, gClient, pkt.id());
}

nlohmann::json GameRoom::ToJson()
{
	nlohmann::json json;

	json["RoomId"] = roomId;

	return json;
}

void GameRoom::SetScene(shared_ptr<ClientBase> client, string sceneId)
{
	auto gClient = static_pointer_cast<GameClient>(client);

	Protocol::S_BASE_SET_SCENE res;

	auto scene = scenes.find(sceneId);
	if (scene == scenes.end())
	{
		res.set_success(false);
		client->Send(ClientPacketHandler::MakeSendBuffer(res));
		return;
	}

	//기존에 입장한 Scene 이 있을 경우 Leave
	if (gClient->scene != nullptr)
	{
		auto prevScene = gClient->scene;
		gClient->scene = scene->second;
		prevScene->DoAsync(&Scene::Leave, gClient);
		scene->second->DoAsync(&Scene::AddClient, gClient);
	}
	else
	{
		gClient->scene = scene->second;
		scene->second->DoAsync(&Scene::AddClient, gClient);
	}

	res.set_success(true);
	client->Send(ClientPacketHandler::MakeSendBuffer(res));
}