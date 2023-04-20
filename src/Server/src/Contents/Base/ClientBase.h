#pragma once

#include "../../pch.h"

class RoomBase;
class GameSession;
class SendBuffer;

enum class ClientState
{
	NORMAL,
	LEAVING
};

class ClientBase : public JobQueue
{
public:
	~ClientBase()
	{
		GLogManager->Log("Client Destroyed :			", clientId);
	}

	void Leave(string code);
	void Send(shared_ptr<SendBuffer> sendBuffer);

	void ReEnter(shared_ptr<GameSession> session);

	void OnDisconnected();

public:
	string clientId;
	string nickname;
	shared_ptr<RoomBase> enteredRoom = nullptr;
	ClientState state;

private:
	shared_ptr<GameSession> session;
};