#pragma once

#include "../Util/pch.h"
#include "../Util/Protocols.h"

class GameSession;
class GameObject;

class Scene : public JobQueue
{
public :
	Scene(string id);
	~Scene();

	void Clear();

	void Leave(shared_ptr<GameSession> session);

	void AddSession(shared_ptr<GameSession> session);
	void InstantiateObject(shared_ptr<GameSession> session, Protocol::C_BASE_INSTANTIATE_OBJECT pkt);
	void GetObjects(shared_ptr<GameSession> session);
	void SetTransfrom(shared_ptr<GameSession> session, int objectId, float position_x, float position_y, float position_z, float rotation_x, float rotation_y, float rotation_z);
	void SetAnimation(shared_ptr<GameSession> session, int objectId, string animationId, string animation);
	void SetAnimationOnce(shared_ptr<GameSession> session, int objectId, string animationId, bool isLoop, float blend);

	void GetStates(shared_ptr<GameSession> session);
	void SetState(shared_ptr<GameSession> session, string id, string value);
	void RemoveState(shared_ptr<GameSession> session, string id);

	void Broadcast(shared_ptr<SendBuffer> sendBuffer);

	string id;
	map<int, shared_ptr<GameObject>> gameObjects;

	//게임 내 인터렉션 가능한 사항들에 대한 정보
	map<string, string> states;

	map<string, shared_ptr<GameSession>> sessions;
	int idGenerator = 0;
};

