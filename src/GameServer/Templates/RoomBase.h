#pragma once

#include "../../Util/pch.h"
#include "../../Util/Protocols.h"

#include "../RoomTypes.h"

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
	virtual void Leave(shared_ptr<ClientBase> session);
	void SetNickname(shared_ptr<ClientBase> session, string nickname);
	virtual void GetClient(shared_ptr<ClientBase> session);

	virtual void Broadcast(shared_ptr<SendBuffer> sendBuffer);

	virtual nlohmann::json ToJson() = 0;
	
	string roomId;
	std::atomic<RoomState> state;

	int disconnectedSessionWaitTime = 0;

	int maxPlayerNumber = 0;

	map<string, shared_ptr<ClientBase>> clients;

	RoomType type;

	nlohmann::json roomInfoJson;
	string roomInfo;

public:
	virtual void Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER &pkt) {};
	virtual void Handle_C_LEAVE(shared_ptr<ClientBase>& client, Protocol::C_LEAVE &pkt);
	virtual void Handle_C_SET_NICKNAME(shared_ptr<ClientBase>& client, Protocol::C_SET_NICKNAME& pkt);
	virtual void Handle_C_GET_CLIENT(shared_ptr<ClientBase>& client, Protocol::C_GET_CLIENT& pkt);

{%- for pkt in parser.recv_pkt %}
	{%- if (pkt.name != 'C_ENTER') and (pkt.name != 'C_REENTER') and (pkt.name != 'C_LEAVE') and (pkt.name != 'C_SET_NICKNAME') and (pkt.name != 'C_GET_CLIENT')%}
	virtual void Handle_{{pkt.name}}(shared_ptr<ClientBase>& client, Protocol::{{pkt.name}}&pkt) {};
	{%- endif %}
{%- endfor %}
};