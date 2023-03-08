#pragma once
#include "../Util/pch.h"

class Scene;
class GameObject;
class RoomBase;

enum class SessionState
{
	NORMAL,
	DESTRUCTING
};

using namespace std;

class GameSession : public PacketSession
{
public:
	GameSession(io_context& ioc) : PacketSession(ioc), state(SessionState::NORMAL)
	{}
	~GameSession() { 
		GLogManager->Log("Session Destroyed :			", clientId);
		this->OnDestroyed();
	}

	bool Disconnect(string code);

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(unsigned char* buffer, int len) override;

public:
	string clientId;
	string nickname;
	
	shared_ptr<RoomBase> enteredRoom = nullptr;

	shared_ptr<Scene> scene = nullptr;
	std::vector<int> gameObjectIds;
	
	std::atomic<SessionState> state;

	std::function<void()> OnDestroyed = []() {};
};