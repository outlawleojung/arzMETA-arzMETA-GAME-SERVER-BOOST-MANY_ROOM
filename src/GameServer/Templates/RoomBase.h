#pragma once

#include "../../Util/pch.h"
#include "../../Util/Protocols.h"

#include <nlohmann/json.hpp>

class GameSession;
class ClientBase;

enum class RoomState
{
	Idle,
	Running,
	Closing,
	Closed
};

enum class RoomType
{
	Main,
	MyRoom,
	Office
};

class RoomBase : public JobQueue
{
public:
	RoomBase();
	virtual ~RoomBase();

	virtual void Init();
	virtual void Close();
	virtual void HandleClose();
	virtual void Clear() = 0;
	
	virtual void Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt) = 0;
	virtual void Leave(shared_ptr<ClientBase> session) = 0;

	virtual void Broadcast(shared_ptr<SendBuffer> sendBuffer) = 0;

	virtual nlohmann::json ToJson() = 0;
	
	string roomId;
	std::atomic<RoomState> state;

	int disconnectedSessionWaitTime = 0;

	int maxPlayerNumber = 0;

	map<string, shared_ptr<ClientBase>> clients;

	RoomType type;

public:
	virtual void Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER &pkt) {};
	virtual void Handle_C_LEAVE(shared_ptr<ClientBase>& client, Protocol::C_LEAVE &pkt);

{%- for pkt in parser.recv_pkt %}
	{%- if (pkt.name != 'C_ENTER') and (pkt.name != 'C_REENTER') and (pkt.name != 'C_LEAVE') %}
	virtual void Handle_{{pkt.name}}(shared_ptr<ClientBase>& client, Protocol::{{pkt.name}}&pkt) {};
	{%- endif %}
{%- endfor %}
};