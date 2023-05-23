#pragma once

#include "PacketHandler.h"
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

class RoomBase : public PacketHandler
{
public:
	RoomBase();
	virtual ~RoomBase();

	virtual void Init();
	virtual void Close();
	virtual void HandleClose();
	
	virtual void Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt);
	virtual void Leave(shared_ptr<ClientBase> session);
	void SetNickname(shared_ptr<ClientBase> session, string nickname);
	virtual void GetClient(shared_ptr<ClientBase> session);
	void HandleChat(shared_ptr<ClientBase> session, string chat);

	virtual void Broadcast(shared_ptr<SendBuffer> sendBuffer);
	
	virtual shared_ptr<ClientBase> MakeClient(string clientId, int sessionId);
	
	void SetDefaultClientData(shared_ptr<ClientBase> client);
	
	virtual void SetClientData(shared_ptr<ClientBase> client) {};
	virtual pair<bool, string> HandleEnter(const Protocol::C_ENTER& pkt) { return { false, "DEFAULT_HANDLE_ENTER" }; }
	virtual void OnEnterSuccess(shared_ptr<ClientBase> client);

	RoomType type;
	string roomId;
	string sceneName;
	std::atomic<RoomState> state;
	nlohmann::json roomInfo;

	map<string, shared_ptr<ClientBase>> clients;

	int disconnectedSessionWaitTime = 0;
	int maxPlayerNumber = 0;

public:
	virtual void Handle_C_LEAVE(shared_ptr<ClientBase>& client, Protocol::C_LEAVE &pkt);
	virtual void Handle_C_SET_NICKNAME(shared_ptr<ClientBase>& client, Protocol::C_SET_NICKNAME& pkt);
	virtual void Handle_C_GET_CLIENT(shared_ptr<ClientBase>& client, Protocol::C_GET_CLIENT& pkt);
	virtual void Handle_C_CHAT(shared_ptr<ClientBase>& client, Protocol::C_CHAT& pkt);
	virtual void Handle_C_WILDCARD(shared_ptr<ClientBase>& client, Protocol::C_WILDCARD& pkt);
};