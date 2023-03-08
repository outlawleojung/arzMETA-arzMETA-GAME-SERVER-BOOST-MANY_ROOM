#include "GameRoom.h"

#include "../ClientPacketHandler.h"

#include "../Session/GameSession.h"

#include "../GameContents/Scene.h"

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

void GameRoom::ReEnter(shared_ptr<GameSession> session, string clientId)
{
	//TODO
}

void GameRoom::Leave(shared_ptr<GameSession> session)
{
	session->Disconnect("LEAVED");

	if (session->scene != nullptr)
		//Scene 의 Leave 는 직접 호출할 수 없고 DoAsync 를 이용해야 함
		session->scene->DoAsync(&Scene::Leave, session);
}

void GameRoom::Handle_C_BASE_SET_SCENE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_SCENE& pkt) { DoAsync(&GameRoom::SetScene, session, pkt.sceneid()); }
void GameRoom::Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt)
{ 
	if (session->scene == nullptr)
		return;
	session->scene->DoAsync(&Scene::InstantiateObject, session, pkt);
}
void GameRoom::Handle_C_BASE_GET_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_GET_OBJECT& pkt)
{
	if (session->scene == nullptr)
		return;
	session->scene->DoAsync(&Scene::GetObjects, session);
}
void GameRoom::Handle_C_BASE_SET_TRANSFORM(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_TRANSFORM& pkt) 
{ 
	if (session->scene == nullptr)
		return;
	session->scene->DoAsync(&Scene::SetTransfrom, session, pkt.objectid(), pkt.position().x(), pkt.position().y(), pkt.position().z(), pkt.rotation().x(), pkt.rotation().y(), pkt.rotation().z());
}
void GameRoom::Handle_C_BASE_SET_ANIMATION(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION& pkt)
{
	if (session->scene == nullptr)
		return;
	session->scene->DoAsync(&Scene::SetAnimation, session, pkt.objectid(), pkt.animationid(), pkt.animation());
}
void GameRoom::Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt) 
{
	if (session->scene == nullptr)
		return;
	session->scene->DoAsync(&Scene::SetAnimationOnce, session, pkt.objectid(), pkt.animationid(), pkt.isloop(), pkt.blend());
}

void GameRoom::Handle_C_INTERACTION_GET_ITEMS(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_GET_ITEMS& pkt)
{
	if (session->scene == nullptr)
		return;
	session->scene->DoAsync(&Scene::GetStates, session);
}

void GameRoom::Handle_C_INTERACTION_SET_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_SET_ITEM& pkt)
{
	if (session->scene == nullptr)
		return;
	session->scene->DoAsync(&Scene::SetState, session, pkt.id(), pkt.state());
}

void GameRoom::Handle_C_INTERACTION_REMOVE_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_REMOVE_ITEM& pkt)
{
	if (session->scene == nullptr)
		return;
	session->scene->DoAsync(&Scene::RemoveState, session, pkt.id());
}

void GameRoom::SetScene(shared_ptr<GameSession> session, string sceneId)
{
	auto scene = scenes.find(sceneId);
	if (scene == scenes.end())
		return;

	//기존에 입장한 Scene 이 있을 경우 Leave
	if (session->scene != nullptr)
		session->scene->DoAsync(&Scene::Leave, session);

	session->scene = scene->second;
	scene->second->DoAsync(&Scene::AddSession, session);

	Protocol::S_BASE_SET_SCENE res;
	res.set_success(true);
	session->Send(ClientPacketHandler::MakeSendBuffer(res));
}