//#pragma once
//
//#include "../../pch.h"
//#include "../../Util/Protocols.h"
//
//class GameClient;
//class GameObject;
//
//class Scene : public JobQueue
//{
//public :
//	Scene(string id);
//	~Scene();
//
//	void Clear();
//
//	void Leave(shared_ptr<GameClient> client);
//
//	void Enter(shared_ptr<GameClient> client);
//	void InstantiateObject(shared_ptr<GameClient> client, Protocol::C_BASE_INSTANTIATE_OBJECT pkt);
//	void RemoveObject(shared_ptr<GameClient> client);
//	void GetObjects(shared_ptr<GameClient> client);
//	void SetTransfrom(int objectId, float position_x, float position_y, float position_z, float rotation_x, float rotation_y, float rotation_z);
//	void SetAnimation(int objectId, string animationId, string animation);
//	void SetAnimationOnce(int objectId, string animationId, bool isLoop, float blend);
//
//	void GetStates(shared_ptr<GameClient> client);
//	void SetState(shared_ptr<GameClient> client, string id, string value);
//	void RemoveState(shared_ptr<GameClient> client, string id);
//
//	void Broadcast(shared_ptr<SendBuffer> sendBuffer);
//
//	string id;
//	map<int, shared_ptr<GameObject>> gameObjects;
//	map<string, vector<int>> client_gameObjects;
//
//	//게임 내 인터렉션 가능한 사항들에 대한 정보
//	map<string, string> states;
//
//	map<string, shared_ptr<GameClient>> clients;
//	int idGenerator = 0;
//};
//
