#include "PacketManager.h"
#include "Session/GameSession.h"
#include "Contents/ClientManager.h"
#include "Contents/Base/ClientBase.h"
#include "Contents/RoomManager.h"
#include "Contents/Base/RoomBase.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(shared_ptr<GameSession>& session, unsigned char* buffer, int len)
{
	return false;
}

bool Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt)
{
	auto room = GRoomManager->rooms.find(pkt.roomid());
	if (room == GRoomManager->rooms.end())
	{
		Protocol::S_ENTER res;
		res.set_result("WRONG_ROOM_ID");
		session->Send(PacketManager::MakeSendBuffer(res));
		return false;
	}
	room->second->Handle_C_ENTER(session, pkt);
	return true;
}

bool Handle_C_REENTER(shared_ptr<GameSession>& session, Protocol::C_REENTER& pkt)
{
	auto client = GClientManager->GetClient(pkt.clientid());
	if (client == nullptr)
	{
		Protocol::S_REENTER res;
		res.set_success(false);
		session->Send(PacketManager::MakeSendBuffer(res));
		return false;
	}
	client->DoAsync(&ClientBase::ReEnter, session);

	return true;
}

bool Handle_C_LEAVE(shared_ptr<GameSession>& session, Protocol::C_LEAVE& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_LEAVE(session->owner, pkt);

	return true;
}
bool Handle_C_GET_CLIENT(shared_ptr<GameSession>& session, Protocol::C_GET_CLIENT& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_GET_CLIENT(session->owner, pkt);

	return true;
}
bool Handle_C_SET_NICKNAME(shared_ptr<GameSession>& session, Protocol::C_SET_NICKNAME& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_SET_NICKNAME(session->owner, pkt);

	return true;
}
bool Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_BASE_INSTANTIATE_OBJECT(session->owner, pkt);

	return true;
}
bool Handle_C_BASE_GET_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_GET_OBJECT& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_BASE_GET_OBJECT(session->owner, pkt);

	return true;
}
bool Handle_C_BASE_SET_OBJECT_DATA(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_OBJECT_DATA& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_BASE_SET_OBJECT_DATA(session->owner, pkt);

	return true;
}
bool Handle_C_BASE_SET_TRANSFORM(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_TRANSFORM& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_BASE_SET_TRANSFORM(session->owner, pkt);

	return true;
}
bool Handle_C_BASE_SET_ANIMATION(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_BASE_SET_ANIMATION(session->owner, pkt);

	return true;
}
bool Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_BASE_SET_ANIMATION_ONCE(session->owner, pkt);

	return true;
}
bool Handle_C_INTERACTION_GET_ITEMS(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_GET_ITEMS& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_INTERACTION_GET_ITEMS(session->owner, pkt);

	return true;
}
bool Handle_C_INTERACTION_SET_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_SET_ITEM& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_INTERACTION_SET_ITEM(session->owner, pkt);

	return true;
}
bool Handle_C_INTERACTION_REMOVE_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_REMOVE_ITEM& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_INTERACTION_REMOVE_ITEM(session->owner, pkt);

	return true;
}
bool Handle_C_MYROOM_GET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_GET_ROOMINFO& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_MYROOM_GET_ROOMINFO(session->owner, pkt);

	return true;
}
bool Handle_C_MYROOM_SET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_SET_ROOMINFO& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_MYROOM_SET_ROOMINFO(session->owner, pkt);

	return true;
}
bool Handle_C_MYROOM_OTHER_ROOM_LIST(shared_ptr<GameSession>& session, Protocol::C_MYROOM_OTHER_ROOM_LIST& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_MYROOM_OTHER_ROOM_LIST(session->owner, pkt);

	return true;
}
bool Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_WAITING_LIST& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OFFICE_GET_WAITING_LIST(session->owner, pkt);

	return true;
}
bool Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_ACCEPT_WAIT& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OFFICE_ACCEPT_WAIT(session->owner, pkt);

	return true;
}
bool Handle_C_OFFICE_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_HOST& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OFFICE_GET_HOST(session->owner, pkt);

	return true;
}
bool Handle_C_OFFICE_BREAK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_BREAK& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OFFICE_BREAK(session->owner, pkt);

	return true;
}
bool Handle_C_OFFICE_KICK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_KICK& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OFFICE_KICK(session->owner, pkt);

	return true;
}
bool Handle_C_OFFICE_GET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_PERMISSION& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OFFICE_GET_PERMISSION(session->owner, pkt);

	return true;
}
bool Handle_C_OFFICE_SET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_PERMISSION& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OFFICE_SET_PERMISSION(session->owner, pkt);

	return true;
}
bool Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_ROOM_INFO& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OFFICE_SET_ROOM_INFO(session->owner, pkt);

	return true;
}
bool Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_ROOM_INFO& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OFFICE_GET_ROOM_INFO(session->owner, pkt);

	return true;
}
bool Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<GameSession>& session, Protocol::C_OFFICE_VIDEO_STREAM& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OFFICE_VIDEO_STREAM(session->owner, pkt);

	return true;
}
bool Handle_C_OFFICE_SHARE(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SHARE& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OFFICE_SHARE(session->owner, pkt);

	return true;
}
bool Handle_C_MATCHING_START(shared_ptr<GameSession>& session, Protocol::C_MATCHING_START& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_MATCHING_START(session->owner, pkt);

	return true;
}
bool Handle_C_MATCHING_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_MATCHING_GET_HOST& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_MATCHING_GET_HOST(session->owner, pkt);

	return true;
}
bool Handle_C_MATCHING_DIE(shared_ptr<GameSession>& session, Protocol::C_MATCHING_DIE& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_MATCHING_DIE(session->owner, pkt);

	return true;
}
bool Handle_C_OX_START(shared_ptr<GameSession>& session, Protocol::C_OX_START& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OX_START(session->owner, pkt);

	return true;
}
bool Handle_C_OX_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_OX_GET_HOST& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OX_GET_HOST(session->owner, pkt);

	return true;
}
bool Handle_C_OX_DIE(shared_ptr<GameSession>& session, Protocol::C_OX_DIE& pkt)
{
	if (session->owner == nullptr || session->owner->enteredRoom == nullptr || session->owner->enteredRoom->state != RoomState::Running)
		return false;

	session->owner->enteredRoom->Handle_C_OX_DIE(session->owner, pkt);

	return true;
}