#pragma once

#include "../pch.h"

#include <sio_client.h>

class GameSession;
class SendBuffer;

class connection_listener;

class Client : public JobQueue
{
public:
	Client(string clientId)
		: clientId(clientId)
	{}
	~Client()
	{
		GLogManager->Log("Client ", clientId, " Removed");
	}

	void Send(shared_ptr<SendBuffer> sendBuffer);
	void OnDisconnected();

	void StartMove();

public:
	string clientId;
	shared_ptr<GameSession> session;

	int objectId;
	float position_x = 0;
	float position_y = 0;
	float position_z = 0;

	float start_x = 0;
	float start_y = 0;
	float start_z = 0;

	float cap = 10;

	bool isMoving = false;

	//web socket
public:
	void ConnectWS();
	void CloseWS();
	void Register();
	void TestSend();

	void SendWS(string name, sio::message::list list);

private:
	sio::client h;

	std::shared_ptr<connection_listener> l;

	std::mutex _lock;
	std::condition_variable_any _cond;
	bool connect_finish = false;
};