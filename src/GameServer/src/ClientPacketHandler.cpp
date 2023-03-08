#include "ClientPacketHandler.h"
#include "./Room/RoomBase.h"
#include "./Room/RoomManager.h"
#include "./Session/GameSession.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(shared_ptr<GameSession>& session, unsigned char* buffer, int len)
{
	return false;
}

bool Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt)
{
	session->clientId = pkt.clientid();
	session->nickname = pkt.nickname();

	auto room = GRoomManager->rooms.find(pkt.roomid());
	if (room == GRoomManager->rooms.end())
	{
		Protocol::S_ENTER res;
		res.set_result("WRONG_ROOM_ID");
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		return false;
	}

	session->enteredRoom = room->second;
	session->enteredRoom->Handle_C_ENTER(session, pkt);

	return true;
}
bool Handle_C_REENTER(shared_ptr<GameSession>& session, Protocol::C_REENTER& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_REENTER(session, pkt);
	
	return true;
}
bool Handle_C_LEAVE(shared_ptr<GameSession>& session, Protocol::C_LEAVE& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_LEAVE(session, pkt);
	
	return true;
}
bool Handle_C_SET_NICKNAME(shared_ptr<GameSession>& session, Protocol::C_SET_NICKNAME& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_SET_NICKNAME(session, pkt);
	
	return true;
}
bool Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_WAITING_LIST& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_OFFICE_GET_WAITING_LIST(session, pkt);
	
	return true;
}
bool Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_ACCEPT_WAIT& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_OFFICE_ACCEPT_WAIT(session, pkt);
	
	return true;
}
bool Handle_C_OFFICE_GET_CLIENT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_CLIENT& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_OFFICE_GET_CLIENT(session, pkt);
	
	return true;
}
bool Handle_C_OFFICE_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_HOST& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_OFFICE_GET_HOST(session, pkt);
	
	return true;
}
bool Handle_C_OFFICE_BREAK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_BREAK& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_OFFICE_BREAK(session, pkt);
	
	return true;
}
bool Handle_C_OFFICE_KICK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_KICK& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_OFFICE_KICK(session, pkt);
	
	return true;
}
bool Handle_C_OFFICE_GET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_PERMISSION& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_OFFICE_GET_PERMISSION(session, pkt);
	
	return true;
}
bool Handle_C_OFFICE_SET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_PERMISSION& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_OFFICE_SET_PERMISSION(session, pkt);
	
	return true;
}
bool Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_ROOM_INFO& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_OFFICE_SET_ROOM_INFO(session, pkt);
	
	return true;
}
bool Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_ROOM_INFO& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_OFFICE_GET_ROOM_INFO(session, pkt);
	
	return true;
}
bool Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<GameSession>& session, Protocol::C_OFFICE_VIDEO_STREAM& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_OFFICE_VIDEO_STREAM(session, pkt);
	
	return true;
}
bool Handle_C_MYROOM_GET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_GET_ROOMINFO& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_MYROOM_GET_ROOMINFO(session, pkt);
	
	return true;
}
bool Handle_C_MYROOM_SET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_SET_ROOMINFO& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_MYROOM_SET_ROOMINFO(session, pkt);
	
	return true;
}
bool Handle_C_MYROOM_OTHER_ROOM_LIST(shared_ptr<GameSession>& session, Protocol::C_MYROOM_OTHER_ROOM_LIST& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_MYROOM_OTHER_ROOM_LIST(session, pkt);
	
	return true;
}
bool Handle_C_BASE_SET_SCENE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_SCENE& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_BASE_SET_SCENE(session, pkt);
	
	return true;
}
bool Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_BASE_INSTANTIATE_OBJECT(session, pkt);
	
	return true;
}
bool Handle_C_BASE_GET_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_GET_OBJECT& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_BASE_GET_OBJECT(session, pkt);
	
	return true;
}
bool Handle_C_BASE_SET_OBJECT_DATA(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_OBJECT_DATA& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_BASE_SET_OBJECT_DATA(session, pkt);
	
	return true;
}
bool Handle_C_BASE_SET_TRANSFORM(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_TRANSFORM& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_BASE_SET_TRANSFORM(session, pkt);
	
	return true;
}
bool Handle_C_BASE_SET_ANIMATION(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_BASE_SET_ANIMATION(session, pkt);
	
	return true;
}
bool Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_BASE_SET_ANIMATION_ONCE(session, pkt);
	
	return true;
}
bool Handle_C_INTERACTION_GET_ITEMS(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_GET_ITEMS& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_INTERACTION_GET_ITEMS(session, pkt);
	
	return true;
}
bool Handle_C_INTERACTION_SET_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_SET_ITEM& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_INTERACTION_SET_ITEM(session, pkt);
	
	return true;
}
bool Handle_C_INTERACTION_REMOVE_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_REMOVE_ITEM& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_C_INTERACTION_REMOVE_ITEM(session, pkt);
	
	return true;
}