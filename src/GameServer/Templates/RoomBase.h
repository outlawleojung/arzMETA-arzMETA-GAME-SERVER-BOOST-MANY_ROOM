#pragma once

#include "../Util/pch.h"
#include "../Util/Protocols.h"

#include <nlohmann/json.hpp>

class GameSession;

enum class RoomState
{
	Idle,
	Running,
	Closing,
	Closed
};

class RoomBase : public JobQueue
{
public:
	RoomBase();
	virtual ~RoomBase();

	virtual void Init();
	virtual void Close();
	virtual void HandleClose() = 0;
	virtual void Clear() = 0;
	
	virtual void Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt) = 0;
	virtual void ReEnter(shared_ptr<GameSession> session, string clientId) = 0;	
	virtual void Leave(shared_ptr<GameSession> session) = 0;

	virtual void Broadcast(shared_ptr<SendBuffer> sendBuffer) = 0;

	virtual nlohmann::json ToJson() = 0;
	
	string roomId;
	std::atomic<RoomState> state;
	std::function<void(std::shared_ptr<GameSession>)> onDisconnected;

public:
{%- for pkt in parser.recv_pkt %}
	virtual void Handle_{{pkt.name}}(shared_ptr<GameSession>& session, Protocol::{{pkt.name}}&pkt) {};
{%- endfor %}
};