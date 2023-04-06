#pragma once

#include "../Base/RoomBase.h"

class Scene;
class ClientBase;

class GameRoom : public RoomBase
{
public :
	GameRoom(vector<string> scenes);

	virtual void Init() override;
	virtual void HandleClose() override;
	virtual void Clear() override;

	virtual void Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) override;

	virtual void Handle_C_BASE_SET_SCENE(shared_ptr<ClientBase>& session, Protocol::C_BASE_SET_SCENE& pkt) override;
	virtual void Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<ClientBase>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt) override;
	virtual void Handle_C_BASE_GET_OBJECT(shared_ptr<ClientBase>& session, Protocol::C_BASE_GET_OBJECT& pkt) override;
	virtual void Handle_C_BASE_SET_TRANSFORM(shared_ptr<ClientBase>& session, Protocol::C_BASE_SET_TRANSFORM& pkt) override;
	virtual void Handle_C_BASE_SET_ANIMATION(shared_ptr<ClientBase>& session, Protocol::C_BASE_SET_ANIMATION& pkt) override;
	virtual void Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<ClientBase>& session, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt) override;

	virtual void Handle_C_INTERACTION_GET_ITEMS(shared_ptr<ClientBase>& session, Protocol::C_INTERACTION_GET_ITEMS& pkt) override;
	virtual void Handle_C_INTERACTION_SET_ITEM(shared_ptr<ClientBase>& session, Protocol::C_INTERACTION_SET_ITEM& pkt) override;
	virtual void Handle_C_INTERACTION_REMOVE_ITEM(shared_ptr<ClientBase>& session, Protocol::C_INTERACTION_REMOVE_ITEM& pkt) override;

	virtual void Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt) override;
	virtual void Leave(shared_ptr<ClientBase> client) override;

	void RemoveObject(shared_ptr<ClientBase> client);

	virtual nlohmann::json ToJson() override;

protected:
	std::map<string, shared_ptr<Scene>> scenes;

	void SetScene(shared_ptr<ClientBase> session, string sceneId);
};
