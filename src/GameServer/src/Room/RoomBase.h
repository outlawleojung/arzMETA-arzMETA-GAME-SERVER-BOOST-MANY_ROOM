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
	virtual void Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER&pkt) {};
	virtual void Handle_C_REENTER(shared_ptr<GameSession>& session, Protocol::C_REENTER&pkt) {};
	virtual void Handle_C_LEAVE(shared_ptr<GameSession>& session, Protocol::C_LEAVE&pkt) {};
	virtual void Handle_C_SET_NICKNAME(shared_ptr<GameSession>& session, Protocol::C_SET_NICKNAME&pkt) {};
	virtual void Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_WAITING_LIST&pkt) {};
	virtual void Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_ACCEPT_WAIT&pkt) {};
	virtual void Handle_C_OFFICE_GET_CLIENT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_CLIENT&pkt) {};
	virtual void Handle_C_OFFICE_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_HOST&pkt) {};
	virtual void Handle_C_OFFICE_BREAK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_BREAK&pkt) {};
	virtual void Handle_C_OFFICE_KICK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_KICK&pkt) {};
	virtual void Handle_C_OFFICE_GET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_PERMISSION&pkt) {};
	virtual void Handle_C_OFFICE_SET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_PERMISSION&pkt) {};
	virtual void Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_ROOM_INFO&pkt) {};
	virtual void Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_ROOM_INFO&pkt) {};
	virtual void Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<GameSession>& session, Protocol::C_OFFICE_VIDEO_STREAM&pkt) {};
	virtual void Handle_C_MYROOM_GET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_GET_ROOMINFO&pkt) {};
	virtual void Handle_C_MYROOM_SET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_SET_ROOMINFO&pkt) {};
	virtual void Handle_C_MYROOM_OTHER_ROOM_LIST(shared_ptr<GameSession>& session, Protocol::C_MYROOM_OTHER_ROOM_LIST&pkt) {};
	virtual void Handle_C_BASE_SET_SCENE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_SCENE&pkt) {};
	virtual void Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_INSTANTIATE_OBJECT&pkt) {};
	virtual void Handle_C_BASE_GET_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_GET_OBJECT&pkt) {};
	virtual void Handle_C_BASE_SET_OBJECT_DATA(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_OBJECT_DATA&pkt) {};
	virtual void Handle_C_BASE_SET_TRANSFORM(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_TRANSFORM&pkt) {};
	virtual void Handle_C_BASE_SET_ANIMATION(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION&pkt) {};
	virtual void Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION_ONCE&pkt) {};
	virtual void Handle_C_INTERACTION_GET_ITEMS(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_GET_ITEMS&pkt) {};
	virtual void Handle_C_INTERACTION_SET_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_SET_ITEM&pkt) {};
	virtual void Handle_C_INTERACTION_REMOVE_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_REMOVE_ITEM&pkt) {};
};