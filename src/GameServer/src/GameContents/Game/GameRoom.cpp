#include "GameRoom.h"

#include "../../ClientPacketHandler.h"
#include "GameClient.h"
#include "Scene.h"

GameRoom::GameRoom(vector<string> sceneIds)
{
	for (auto sceneId = sceneIds.begin(); sceneId != sceneIds.end(); sceneId++)
		scenes.insert({ *sceneId, make_shared<Scene>(*sceneId) });
}

void GameRoom::HandleClose()
{
	//Clear();
}

void GameRoom::Clear()
{
	for (auto scene = scenes.begin(); scene != scenes.end(); scene++)
		scene->second->Clear();

	scenes.clear();
}

void GameRoom::Leave(shared_ptr<ClientBase> client)
{
	auto gClient = static_pointer_cast<GameClient>(client);

	if (gClient->scene != nullptr)
		//Scene 의 Leave 는 직접 호출할 수 없고 DoAsync 를 이용해야 함
		gClient->scene->DoAsync(&Scene::Leave, gClient);
}

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

void GameRoom::SetScene(shared_ptr<ClientBase> client, string sceneId)
{
	auto gClient = static_pointer_cast<GameClient>(client);

	auto scene = scenes.find(sceneId);
	if (scene == scenes.end())
		return;

	//기존에 입장한 Scene 이 있을 경우 Leave
	if (gClient->scene != nullptr)
		gClient->scene->DoAsync(&Scene::Leave, gClient);

	gClient->scene = scene->second;
	scene->second->DoAsync(&Scene::AddClient, gClient);

	Protocol::S_BASE_SET_SCENE res;
	res.set_success(true);
	client->Send(ClientPacketHandler::MakeSendBuffer(res));
}