#pragma once

#include "../../pch.h"
#include "../../Protocols.h"

class GameSession;
class ClientBase;

class PacketHandler : public JobQueue
{
public:
	virtual void Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) {};
	virtual void Handle_C_REENTER(shared_ptr<GameSession>& session, Protocol::C_REENTER& pkt) {};
	virtual void Handle_C_LEAVE(shared_ptr<ClientBase>& client, Protocol::C_LEAVE& pkt) {};
	virtual void Handle_C_GET_CLIENT(shared_ptr<ClientBase>& client, Protocol::C_GET_CLIENT& pkt) {};
	virtual void Handle_C_SET_NICKNAME(shared_ptr<ClientBase>& client, Protocol::C_SET_NICKNAME& pkt) {};
	virtual void Handle_C_CHAT(shared_ptr<ClientBase>& client, Protocol::C_CHAT& pkt) {};
	virtual void Handle_C_WILDCARD(shared_ptr<ClientBase>& client, Protocol::C_WILDCARD& pkt) {};
	virtual void Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<ClientBase>& client, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt) {};
	virtual void Handle_C_BASE_REMOVE_OBJECT(shared_ptr<ClientBase>& client, Protocol::C_BASE_REMOVE_OBJECT& pkt) {};
	virtual void Handle_C_BASE_GET_OBJECT(shared_ptr<ClientBase>& client, Protocol::C_BASE_GET_OBJECT& pkt) {};
	virtual void Handle_C_BASE_SET_OBJECT_DATA(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_OBJECT_DATA& pkt) {};
	virtual void Handle_C_BASE_SET_TRANSFORM(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_TRANSFORM& pkt) {};
	virtual void Handle_C_BASE_SET_ANIMATION(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_ANIMATION& pkt) {};
	virtual void Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<ClientBase>& client, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt) {};
	virtual void Handle_C_INTERACTION_GET_ITEMS(shared_ptr<ClientBase>& client, Protocol::C_INTERACTION_GET_ITEMS& pkt) {};
	virtual void Handle_C_INTERACTION_SET_ITEM(shared_ptr<ClientBase>& client, Protocol::C_INTERACTION_SET_ITEM& pkt) {};
	virtual void Handle_C_INTERACTION_REMOVE_ITEM(shared_ptr<ClientBase>& client, Protocol::C_INTERACTION_REMOVE_ITEM& pkt) {};
	virtual void Handle_C_MYROOM_GET_ROOMINFO(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_GET_ROOMINFO& pkt) {};
	virtual void Handle_C_MYROOM_SET_ROOMINFO(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_SET_ROOMINFO& pkt) {};
	virtual void Handle_C_MYROOM_OTHER_ROOM_LIST(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_OTHER_ROOM_LIST& pkt) {};
	virtual void Handle_C_MYROOM_START_EDIT(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_START_EDIT& pkt) {};
	virtual void Handle_C_MYROOM_END_EDIT(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_END_EDIT& pkt) {};
	virtual void Handle_C_MYROOM_KICK(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_KICK& pkt) {};
	virtual void Handle_C_MYROOM_SHUTDOWN(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_SHUTDOWN& pkt) {};
	virtual void Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_WAITING_LIST& pkt) {};
	virtual void Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_ACCEPT_WAIT& pkt) {};
	virtual void Handle_C_OFFICE_GET_HOST(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_HOST& pkt) {};
	virtual void Handle_C_OFFICE_BREAK(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_BREAK& pkt) {};
	virtual void Handle_C_OFFICE_KICK(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_KICK& pkt) {};
	virtual void Handle_C_OFFICE_GET_PERMISSION(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_PERMISSION& pkt) {};
	virtual void Handle_C_OFFICE_GET_PERMISSION_ALL(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_PERMISSION_ALL& pkt) {};
	virtual void Handle_C_OFFICE_SET_PERMISSION(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_SET_PERMISSION& pkt) {};
	virtual void Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_SET_ROOM_INFO& pkt) {};
	virtual void Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_GET_ROOM_INFO& pkt) {};
	virtual void Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_VIDEO_STREAM& pkt) {};
	virtual void Handle_C_OFFICE_SHARE(shared_ptr<ClientBase>& client, Protocol::C_OFFICE_SHARE& pkt) {};
	virtual void Handle_C_MATCHING_START(shared_ptr<ClientBase>& client, Protocol::C_MATCHING_START& pkt) {};
	virtual void Handle_C_MATCHING_GET_HOST(shared_ptr<ClientBase>& client, Protocol::C_MATCHING_GET_HOST& pkt) {};
	virtual void Handle_C_MATCHING_DIE(shared_ptr<ClientBase>& client, Protocol::C_MATCHING_DIE& pkt) {};
	virtual void Handle_C_OX_START(shared_ptr<ClientBase>& client, Protocol::C_OX_START& pkt) {};
	virtual void Handle_C_OX_GET_HOST(shared_ptr<ClientBase>& client, Protocol::C_OX_GET_HOST& pkt) {};
	virtual void Handle_C_OX_DIE(shared_ptr<ClientBase>& client, Protocol::C_OX_DIE& pkt) {};
};