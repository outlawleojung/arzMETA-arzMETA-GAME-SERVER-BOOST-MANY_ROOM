#pragma once

#include "../Base/RoomBase.h"

class ClientBase;
class GameClient;
class GameObject;

class GameRoom : public RoomBase
{
public :
	GameRoom();

	virtual void Init() override;

	virtual void Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) override;

	virtual void Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<ClientBase>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt) override;
	virtual void Handle_C_BASE_REMOVE_OBJECT(shared_ptr<ClientBase>& session, Protocol::C_BASE_REMOVE_OBJECT& pkt) override;
	virtual void Handle_C_BASE_GET_OBJECT(shared_ptr<ClientBase>& session, Protocol::C_BASE_GET_OBJECT& pkt) override;
	virtual void Handle_C_BASE_SET_OBJECT_DATA(shared_ptr<ClientBase>& session, Protocol::C_BASE_SET_OBJECT_DATA& pkt) override;
	virtual void Handle_C_BASE_SET_TRANSFORM(shared_ptr<ClientBase>& session, Protocol::C_BASE_SET_TRANSFORM& pkt) override;
	virtual void Handle_C_BASE_SET_ANIMATION(shared_ptr<ClientBase>& session, Protocol::C_BASE_SET_ANIMATION& pkt) override;
	virtual void Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<ClientBase>& session, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt) override;

	virtual void Leave(shared_ptr<ClientBase> client) override;
	
	virtual shared_ptr<ClientBase> MakeClient(string clientId, int sessionId) override;
	virtual pair<bool, string> HandleEnter(const Protocol::C_ENTER& pkt) override;

	void InstantiateObject(shared_ptr<GameClient> client, Protocol::C_BASE_INSTANTIATE_OBJECT pkt);
	void RemoveObject(shared_ptr<GameClient> client);
	void GetObjects(shared_ptr<GameClient> client);
	void SetObjectData(shared_ptr<GameClient> client, int objectId, string objectData);
	void SetTransfrom(Protocol::C_BASE_SET_TRANSFORM pkt);
	void SetAnimation(int objectId, string animationId, string animation);
	void SetAnimationOnce(int objectId, string animationId, bool isLoop, float blend);

	void ClearObject();

	map<int, shared_ptr<GameObject>> gameObjects;

	int idGenerator = 0;
};
