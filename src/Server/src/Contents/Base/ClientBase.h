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
		GLogManager->Log("Client Destroyed : ", clientId, " from ", enteredRoomId);
	}

	void Leave(string code);
	void Send(shared_ptr<SendBuffer> sendBuffer);

	void Test_Send(shared_ptr<SendBuffer> sendBuffer);

	void ReEnter(shared_ptr<GameSession> session);

	void OnDisconnected();

public:
	string clientId;
	string sessionId;
	string nickname;
	string stateMessage;

	shared_ptr<RoomBase> enteredRoom = nullptr;
	string enteredRoomId;
	ClientState state;

	shared_ptr<GameSession> session;
};