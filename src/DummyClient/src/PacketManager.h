#pragma once

#include "pch.h"
#include "Protocols.h"

class GameSession;

using PacketHandlerFunc = std::function<bool(shared_ptr<GameSession>&, unsigned char*, int)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : unsigned short
{
	PKT_C_ENTER = 0,
	PKT_S_ENTER = 1,
	PKT_C_REENTER = 2,
	PKT_S_REENTER = 3,
	PKT_C_LEAVE = 4,
	PKT_C_GET_CLIENT = 5,
	PKT_S_ADD_CLIENT = 6,
	PKT_S_REMOVE_CLIENT = 7,
	PKT_C_SET_NICKNAME = 8,
	PKT_S_SET_NICKNAME = 9,
	PKT_S_SET_NICKNAME_NOTICE = 10,
	PKT_C_CHAT = 11,
	PKT_S_CHAT = 12,
	PKT_S_DISCONNECT = 13,
	PKT_C_BASE_INSTANTIATE_OBJECT = 100,
	PKT_S_BASE_INSTANTIATE_OBJECT = 101,
	PKT_C_BASE_GET_OBJECT = 102,
	PKT_S_BASE_ADD_OBJECT = 103,
	PKT_S_BASE_REMOVE_OBJECT = 104,
	PKT_C_BASE_SET_OBJECT_DATA = 105,
	PKT_S_BASE_SET_OBJECT_DATA = 106,
	PKT_S_BASE_SET_OBJECT_DATA_NOTICE = 107,
	PKT_C_BASE_SET_TRANSFORM = 108,
	PKT_S_BASE_SET_TRANSFORM = 109,
	PKT_C_BASE_SET_ANIMATION = 110,
	PKT_S_BASE_SET_ANIMATION = 111,
	PKT_C_BASE_SET_ANIMATION_ONCE = 112,
	PKT_S_BASE_SET_ANIMATION_ONCE = 113,
	PKT_C_INTERACTION_GET_ITEMS = 200,
	PKT_S_INTERACTION_GET_ITEMS = 201,
	PKT_C_INTERACTION_SET_ITEM = 202,
	PKT_S_INTERACTION_SET_ITEM = 203,
	PKT_S_INTERACTION_SET_ITEM_NOTICE = 204,
	PKT_C_INTERACTION_REMOVE_ITEM = 205,
	PKT_S_INTERACTION_REMOVE_ITEM = 206,
	PKT_S_INTERACTION_REMOVE_ITEM_NOTICE = 207,
	PKT_C_MYROOM_GET_ROOMINFO = 300,
	PKT_S_MYROOM_GET_ROOMINFO = 301,
	PKT_C_MYROOM_SET_ROOMINFO = 302,
	PKT_S_MYROOM_SET_ROOMINFO = 303,
	PKT_C_MYROOM_OTHER_ROOM_LIST = 304,
	PKT_S_MYROOM_OTHER_ROOM_LIST = 305,
	PKT_C_MYROOM_START_EDIT = 306,
	PKT_S_MYROOM_START_EDIT = 307,
	PKT_C_MYROOM_END_EDIT = 308,
	PKT_S_MYROOM_END_EDIT = 309,
	PKT_C_MYROOM_KICK = 310,
	PKT_S_MYROOM_KICK = 311,
	PKT_C_MYROOM_SHUTDOWN = 312,
	PKT_S_MYROOM_SHUTDOWN = 313,
	PKT_C_OFFICE_GET_WAITING_LIST = 400,
	PKT_S_OFFICE_ADD_WAITING_CLIENT = 401,
	PKT_S_OFFICE_REMOVE_WAITING_CLIENT = 402,
	PKT_C_OFFICE_ACCEPT_WAIT = 403,
	PKT_S_OFFICE_ACCEPT_WAIT = 404,
	PKT_S_OFFICE_ACCEPT_WAIT_NOTICE = 405,
	PKT_C_OFFICE_GET_HOST = 406,
	PKT_S_OFFICE_GET_HOST = 407,
	PKT_C_OFFICE_BREAK = 408,
	PKT_S_OFFICE_BREAK = 409,
	PKT_C_OFFICE_KICK = 410,
	PKT_S_OFFICE_KICK = 411,
	PKT_C_OFFICE_GET_PERMISSION = 412,
	PKT_S_OFFICE_GET_PERMISSION = 413,
	PKT_C_OFFICE_SET_PERMISSION = 414,
	PKT_S_OFFICE_SET_PERMISSION = 415,
	PKT_S_OFFICE_SET_PERMISSION_NOTICE = 416,
	PKT_C_OFFICE_SET_ROOM_INFO = 417,
	PKT_S_OFFICE_SET_ROOM_INFO = 418,
	PKT_C_OFFICE_GET_ROOM_INFO = 419,
	PKT_S_OFFICE_GET_ROOM_INFO = 420,
	PKT_C_OFFICE_VIDEO_STREAM = 421,
	PKT_S_OFFICE_VIDEO_STREAM = 422,
	PKT_C_OFFICE_SHARE = 423,
	PKT_S_OFFICE_SHARE = 424,
	PKT_C_MATCHING_START = 500,
	PKT_S_MATCHING_START = 501,
	PKT_S_MATCHING_FINISH = 502,
	PKT_C_MATCHING_GET_HOST = 503,
	PKT_S_MATCHING_HOST = 504,
	PKT_S_MATCHING_ROUND_START = 505,
	PKT_S_MATCHING_ROUND_FINISH = 506,
	PKT_S_MATCHING_TILES = 507,
	PKT_S_MATCHING_HINT = 508,
	PKT_S_MATCHING_PROBLEM = 509,
	PKT_S_MATCHING_DESTROY = 510,
	PKT_S_MATCHING_QUIZ_DISAPPEAR = 511,
	PKT_C_MATCHING_DIE = 512,
	PKT_C_OX_START = 600,
	PKT_S_OX_START = 601,
	PKT_S_OX_FINISH = 602,
	PKT_C_OX_GET_HOST = 603,
	PKT_S_OX_HOST = 604,
	PKT_S_OX_ROUND_START = 605,
	PKT_S_OX_ROUND_FINISH = 606,
	PKT_S_OX_QUIZ = 607,
	PKT_S_OX_DESTROY = 608,
	PKT_C_OX_DIE = 609,
};

bool Handle_INVALID(shared_ptr<GameSession>& session, unsigned char* buffer, int len);
bool Handle_S_ENTER(shared_ptr<GameSession>& session, Protocol::S_ENTER& pkt);
bool Handle_S_REENTER(shared_ptr<GameSession>& session, Protocol::S_REENTER& pkt);
bool Handle_S_ADD_CLIENT(shared_ptr<GameSession>& session, Protocol::S_ADD_CLIENT& pkt);
bool Handle_S_REMOVE_CLIENT(shared_ptr<GameSession>& session, Protocol::S_REMOVE_CLIENT& pkt);
bool Handle_S_SET_NICKNAME(shared_ptr<GameSession>& session, Protocol::S_SET_NICKNAME& pkt);
bool Handle_S_SET_NICKNAME_NOTICE(shared_ptr<GameSession>& session, Protocol::S_SET_NICKNAME_NOTICE& pkt);
bool Handle_S_CHAT(shared_ptr<GameSession>& session, Protocol::S_CHAT& pkt);
bool Handle_S_DISCONNECT(shared_ptr<GameSession>& session, Protocol::S_DISCONNECT& pkt);
bool Handle_S_BASE_INSTANTIATE_OBJECT(shared_ptr<GameSession>& session, Protocol::S_BASE_INSTANTIATE_OBJECT& pkt);
bool Handle_S_BASE_ADD_OBJECT(shared_ptr<GameSession>& session, Protocol::S_BASE_ADD_OBJECT& pkt);
bool Handle_S_BASE_REMOVE_OBJECT(shared_ptr<GameSession>& session, Protocol::S_BASE_REMOVE_OBJECT& pkt);
bool Handle_S_BASE_SET_OBJECT_DATA(shared_ptr<GameSession>& session, Protocol::S_BASE_SET_OBJECT_DATA& pkt);
bool Handle_S_BASE_SET_OBJECT_DATA_NOTICE(shared_ptr<GameSession>& session, Protocol::S_BASE_SET_OBJECT_DATA_NOTICE& pkt);
bool Handle_S_BASE_SET_TRANSFORM(shared_ptr<GameSession>& session, Protocol::S_BASE_SET_TRANSFORM& pkt);
bool Handle_S_BASE_SET_ANIMATION(shared_ptr<GameSession>& session, Protocol::S_BASE_SET_ANIMATION& pkt);
bool Handle_S_BASE_SET_ANIMATION_ONCE(shared_ptr<GameSession>& session, Protocol::S_BASE_SET_ANIMATION_ONCE& pkt);
bool Handle_S_INTERACTION_GET_ITEMS(shared_ptr<GameSession>& session, Protocol::S_INTERACTION_GET_ITEMS& pkt);
bool Handle_S_INTERACTION_SET_ITEM(shared_ptr<GameSession>& session, Protocol::S_INTERACTION_SET_ITEM& pkt);
bool Handle_S_INTERACTION_SET_ITEM_NOTICE(shared_ptr<GameSession>& session, Protocol::S_INTERACTION_SET_ITEM_NOTICE& pkt);
bool Handle_S_INTERACTION_REMOVE_ITEM(shared_ptr<GameSession>& session, Protocol::S_INTERACTION_REMOVE_ITEM& pkt);
bool Handle_S_INTERACTION_REMOVE_ITEM_NOTICE(shared_ptr<GameSession>& session, Protocol::S_INTERACTION_REMOVE_ITEM_NOTICE& pkt);
bool Handle_S_MYROOM_GET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::S_MYROOM_GET_ROOMINFO& pkt);
bool Handle_S_MYROOM_SET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::S_MYROOM_SET_ROOMINFO& pkt);
bool Handle_S_MYROOM_OTHER_ROOM_LIST(shared_ptr<GameSession>& session, Protocol::S_MYROOM_OTHER_ROOM_LIST& pkt);
bool Handle_S_MYROOM_START_EDIT(shared_ptr<GameSession>& session, Protocol::S_MYROOM_START_EDIT& pkt);
bool Handle_S_MYROOM_END_EDIT(shared_ptr<GameSession>& session, Protocol::S_MYROOM_END_EDIT& pkt);
bool Handle_S_MYROOM_KICK(shared_ptr<GameSession>& session, Protocol::S_MYROOM_KICK& pkt);
bool Handle_S_MYROOM_SHUTDOWN(shared_ptr<GameSession>& session, Protocol::S_MYROOM_SHUTDOWN& pkt);
bool Handle_S_OFFICE_ADD_WAITING_CLIENT(shared_ptr<GameSession>& session, Protocol::S_OFFICE_ADD_WAITING_CLIENT& pkt);
bool Handle_S_OFFICE_REMOVE_WAITING_CLIENT(shared_ptr<GameSession>& session, Protocol::S_OFFICE_REMOVE_WAITING_CLIENT& pkt);
bool Handle_S_OFFICE_ACCEPT_WAIT(shared_ptr<GameSession>& session, Protocol::S_OFFICE_ACCEPT_WAIT& pkt);
bool Handle_S_OFFICE_ACCEPT_WAIT_NOTICE(shared_ptr<GameSession>& session, Protocol::S_OFFICE_ACCEPT_WAIT_NOTICE& pkt);
bool Handle_S_OFFICE_GET_HOST(shared_ptr<GameSession>& session, Protocol::S_OFFICE_GET_HOST& pkt);
bool Handle_S_OFFICE_BREAK(shared_ptr<GameSession>& session, Protocol::S_OFFICE_BREAK& pkt);
bool Handle_S_OFFICE_KICK(shared_ptr<GameSession>& session, Protocol::S_OFFICE_KICK& pkt);
bool Handle_S_OFFICE_GET_PERMISSION(shared_ptr<GameSession>& session, Protocol::S_OFFICE_GET_PERMISSION& pkt);
bool Handle_S_OFFICE_SET_PERMISSION(shared_ptr<GameSession>& session, Protocol::S_OFFICE_SET_PERMISSION& pkt);
bool Handle_S_OFFICE_SET_PERMISSION_NOTICE(shared_ptr<GameSession>& session, Protocol::S_OFFICE_SET_PERMISSION_NOTICE& pkt);
bool Handle_S_OFFICE_SET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::S_OFFICE_SET_ROOM_INFO& pkt);
bool Handle_S_OFFICE_GET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::S_OFFICE_GET_ROOM_INFO& pkt);
bool Handle_S_OFFICE_VIDEO_STREAM(shared_ptr<GameSession>& session, Protocol::S_OFFICE_VIDEO_STREAM& pkt);
bool Handle_S_OFFICE_SHARE(shared_ptr<GameSession>& session, Protocol::S_OFFICE_SHARE& pkt);
bool Handle_S_MATCHING_START(shared_ptr<GameSession>& session, Protocol::S_MATCHING_START& pkt);
bool Handle_S_MATCHING_FINISH(shared_ptr<GameSession>& session, Protocol::S_MATCHING_FINISH& pkt);
bool Handle_S_MATCHING_HOST(shared_ptr<GameSession>& session, Protocol::S_MATCHING_HOST& pkt);
bool Handle_S_MATCHING_ROUND_START(shared_ptr<GameSession>& session, Protocol::S_MATCHING_ROUND_START& pkt);
bool Handle_S_MATCHING_ROUND_FINISH(shared_ptr<GameSession>& session, Protocol::S_MATCHING_ROUND_FINISH& pkt);
bool Handle_S_MATCHING_TILES(shared_ptr<GameSession>& session, Protocol::S_MATCHING_TILES& pkt);
bool Handle_S_MATCHING_HINT(shared_ptr<GameSession>& session, Protocol::S_MATCHING_HINT& pkt);
bool Handle_S_MATCHING_PROBLEM(shared_ptr<GameSession>& session, Protocol::S_MATCHING_PROBLEM& pkt);
bool Handle_S_MATCHING_DESTROY(shared_ptr<GameSession>& session, Protocol::S_MATCHING_DESTROY& pkt);
bool Handle_S_MATCHING_QUIZ_DISAPPEAR(shared_ptr<GameSession>& session, Protocol::S_MATCHING_QUIZ_DISAPPEAR& pkt);
bool Handle_S_OX_START(shared_ptr<GameSession>& session, Protocol::S_OX_START& pkt);
bool Handle_S_OX_FINISH(shared_ptr<GameSession>& session, Protocol::S_OX_FINISH& pkt);
bool Handle_S_OX_HOST(shared_ptr<GameSession>& session, Protocol::S_OX_HOST& pkt);
bool Handle_S_OX_ROUND_START(shared_ptr<GameSession>& session, Protocol::S_OX_ROUND_START& pkt);
bool Handle_S_OX_ROUND_FINISH(shared_ptr<GameSession>& session, Protocol::S_OX_ROUND_FINISH& pkt);
bool Handle_S_OX_QUIZ(shared_ptr<GameSession>& session, Protocol::S_OX_QUIZ& pkt);
bool Handle_S_OX_DESTROY(shared_ptr<GameSession>& session, Protocol::S_OX_DESTROY& pkt);

class PacketManager
{
public:
	static void Init()
	{
		for (int i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_S_ENTER] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_ENTER > (Handle_S_ENTER, session, buffer, len); };
		GPacketHandler[PKT_S_REENTER] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_REENTER > (Handle_S_REENTER, session, buffer, len); };
		GPacketHandler[PKT_S_ADD_CLIENT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_ADD_CLIENT > (Handle_S_ADD_CLIENT, session, buffer, len); };
		GPacketHandler[PKT_S_REMOVE_CLIENT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_REMOVE_CLIENT > (Handle_S_REMOVE_CLIENT, session, buffer, len); };
		GPacketHandler[PKT_S_SET_NICKNAME] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_SET_NICKNAME > (Handle_S_SET_NICKNAME, session, buffer, len); };
		GPacketHandler[PKT_S_SET_NICKNAME_NOTICE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_SET_NICKNAME_NOTICE > (Handle_S_SET_NICKNAME_NOTICE, session, buffer, len); };
		GPacketHandler[PKT_S_CHAT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_CHAT > (Handle_S_CHAT, session, buffer, len); };
		GPacketHandler[PKT_S_DISCONNECT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_DISCONNECT > (Handle_S_DISCONNECT, session, buffer, len); };
		GPacketHandler[PKT_S_BASE_INSTANTIATE_OBJECT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_BASE_INSTANTIATE_OBJECT > (Handle_S_BASE_INSTANTIATE_OBJECT, session, buffer, len); };
		GPacketHandler[PKT_S_BASE_ADD_OBJECT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_BASE_ADD_OBJECT > (Handle_S_BASE_ADD_OBJECT, session, buffer, len); };
		GPacketHandler[PKT_S_BASE_REMOVE_OBJECT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_BASE_REMOVE_OBJECT > (Handle_S_BASE_REMOVE_OBJECT, session, buffer, len); };
		GPacketHandler[PKT_S_BASE_SET_OBJECT_DATA] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_BASE_SET_OBJECT_DATA > (Handle_S_BASE_SET_OBJECT_DATA, session, buffer, len); };
		GPacketHandler[PKT_S_BASE_SET_OBJECT_DATA_NOTICE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_BASE_SET_OBJECT_DATA_NOTICE > (Handle_S_BASE_SET_OBJECT_DATA_NOTICE, session, buffer, len); };
		GPacketHandler[PKT_S_BASE_SET_TRANSFORM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_BASE_SET_TRANSFORM > (Handle_S_BASE_SET_TRANSFORM, session, buffer, len); };
		GPacketHandler[PKT_S_BASE_SET_ANIMATION] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_BASE_SET_ANIMATION > (Handle_S_BASE_SET_ANIMATION, session, buffer, len); };
		GPacketHandler[PKT_S_BASE_SET_ANIMATION_ONCE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_BASE_SET_ANIMATION_ONCE > (Handle_S_BASE_SET_ANIMATION_ONCE, session, buffer, len); };
		GPacketHandler[PKT_S_INTERACTION_GET_ITEMS] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_INTERACTION_GET_ITEMS > (Handle_S_INTERACTION_GET_ITEMS, session, buffer, len); };
		GPacketHandler[PKT_S_INTERACTION_SET_ITEM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_INTERACTION_SET_ITEM > (Handle_S_INTERACTION_SET_ITEM, session, buffer, len); };
		GPacketHandler[PKT_S_INTERACTION_SET_ITEM_NOTICE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_INTERACTION_SET_ITEM_NOTICE > (Handle_S_INTERACTION_SET_ITEM_NOTICE, session, buffer, len); };
		GPacketHandler[PKT_S_INTERACTION_REMOVE_ITEM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_INTERACTION_REMOVE_ITEM > (Handle_S_INTERACTION_REMOVE_ITEM, session, buffer, len); };
		GPacketHandler[PKT_S_INTERACTION_REMOVE_ITEM_NOTICE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_INTERACTION_REMOVE_ITEM_NOTICE > (Handle_S_INTERACTION_REMOVE_ITEM_NOTICE, session, buffer, len); };
		GPacketHandler[PKT_S_MYROOM_GET_ROOMINFO] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MYROOM_GET_ROOMINFO > (Handle_S_MYROOM_GET_ROOMINFO, session, buffer, len); };
		GPacketHandler[PKT_S_MYROOM_SET_ROOMINFO] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MYROOM_SET_ROOMINFO > (Handle_S_MYROOM_SET_ROOMINFO, session, buffer, len); };
		GPacketHandler[PKT_S_MYROOM_OTHER_ROOM_LIST] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MYROOM_OTHER_ROOM_LIST > (Handle_S_MYROOM_OTHER_ROOM_LIST, session, buffer, len); };
		GPacketHandler[PKT_S_MYROOM_START_EDIT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MYROOM_START_EDIT > (Handle_S_MYROOM_START_EDIT, session, buffer, len); };
		GPacketHandler[PKT_S_MYROOM_END_EDIT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MYROOM_END_EDIT > (Handle_S_MYROOM_END_EDIT, session, buffer, len); };
		GPacketHandler[PKT_S_MYROOM_KICK] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MYROOM_KICK > (Handle_S_MYROOM_KICK, session, buffer, len); };
		GPacketHandler[PKT_S_MYROOM_SHUTDOWN] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MYROOM_SHUTDOWN > (Handle_S_MYROOM_SHUTDOWN, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_ADD_WAITING_CLIENT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_ADD_WAITING_CLIENT > (Handle_S_OFFICE_ADD_WAITING_CLIENT, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_REMOVE_WAITING_CLIENT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_REMOVE_WAITING_CLIENT > (Handle_S_OFFICE_REMOVE_WAITING_CLIENT, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_ACCEPT_WAIT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_ACCEPT_WAIT > (Handle_S_OFFICE_ACCEPT_WAIT, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_ACCEPT_WAIT_NOTICE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_ACCEPT_WAIT_NOTICE > (Handle_S_OFFICE_ACCEPT_WAIT_NOTICE, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_GET_HOST] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_GET_HOST > (Handle_S_OFFICE_GET_HOST, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_BREAK] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_BREAK > (Handle_S_OFFICE_BREAK, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_KICK] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_KICK > (Handle_S_OFFICE_KICK, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_GET_PERMISSION] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_GET_PERMISSION > (Handle_S_OFFICE_GET_PERMISSION, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_SET_PERMISSION] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_SET_PERMISSION > (Handle_S_OFFICE_SET_PERMISSION, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_SET_PERMISSION_NOTICE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_SET_PERMISSION_NOTICE > (Handle_S_OFFICE_SET_PERMISSION_NOTICE, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_SET_ROOM_INFO] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_SET_ROOM_INFO > (Handle_S_OFFICE_SET_ROOM_INFO, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_GET_ROOM_INFO] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_GET_ROOM_INFO > (Handle_S_OFFICE_GET_ROOM_INFO, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_VIDEO_STREAM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_VIDEO_STREAM > (Handle_S_OFFICE_VIDEO_STREAM, session, buffer, len); };
		GPacketHandler[PKT_S_OFFICE_SHARE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OFFICE_SHARE > (Handle_S_OFFICE_SHARE, session, buffer, len); };
		GPacketHandler[PKT_S_MATCHING_START] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MATCHING_START > (Handle_S_MATCHING_START, session, buffer, len); };
		GPacketHandler[PKT_S_MATCHING_FINISH] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MATCHING_FINISH > (Handle_S_MATCHING_FINISH, session, buffer, len); };
		GPacketHandler[PKT_S_MATCHING_HOST] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MATCHING_HOST > (Handle_S_MATCHING_HOST, session, buffer, len); };
		GPacketHandler[PKT_S_MATCHING_ROUND_START] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MATCHING_ROUND_START > (Handle_S_MATCHING_ROUND_START, session, buffer, len); };
		GPacketHandler[PKT_S_MATCHING_ROUND_FINISH] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MATCHING_ROUND_FINISH > (Handle_S_MATCHING_ROUND_FINISH, session, buffer, len); };
		GPacketHandler[PKT_S_MATCHING_TILES] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MATCHING_TILES > (Handle_S_MATCHING_TILES, session, buffer, len); };
		GPacketHandler[PKT_S_MATCHING_HINT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MATCHING_HINT > (Handle_S_MATCHING_HINT, session, buffer, len); };
		GPacketHandler[PKT_S_MATCHING_PROBLEM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MATCHING_PROBLEM > (Handle_S_MATCHING_PROBLEM, session, buffer, len); };
		GPacketHandler[PKT_S_MATCHING_DESTROY] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MATCHING_DESTROY > (Handle_S_MATCHING_DESTROY, session, buffer, len); };
		GPacketHandler[PKT_S_MATCHING_QUIZ_DISAPPEAR] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_MATCHING_QUIZ_DISAPPEAR > (Handle_S_MATCHING_QUIZ_DISAPPEAR, session, buffer, len); };
		GPacketHandler[PKT_S_OX_START] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OX_START > (Handle_S_OX_START, session, buffer, len); };
		GPacketHandler[PKT_S_OX_FINISH] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OX_FINISH > (Handle_S_OX_FINISH, session, buffer, len); };
		GPacketHandler[PKT_S_OX_HOST] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OX_HOST > (Handle_S_OX_HOST, session, buffer, len); };
		GPacketHandler[PKT_S_OX_ROUND_START] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OX_ROUND_START > (Handle_S_OX_ROUND_START, session, buffer, len); };
		GPacketHandler[PKT_S_OX_ROUND_FINISH] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OX_ROUND_FINISH > (Handle_S_OX_ROUND_FINISH, session, buffer, len); };
		GPacketHandler[PKT_S_OX_QUIZ] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OX_QUIZ > (Handle_S_OX_QUIZ, session, buffer, len); };
		GPacketHandler[PKT_S_OX_DESTROY] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::S_OX_DESTROY > (Handle_S_OX_DESTROY, session, buffer, len); };
	}

	static bool HandlePacket(shared_ptr<GameSession>& session, unsigned char* buffer, int len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_ENTER& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_REENTER& pkt) { return MakeSendBuffer(pkt, PKT_C_REENTER); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_LEAVE& pkt) { return MakeSendBuffer(pkt, PKT_C_LEAVE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_GET_CLIENT& pkt) { return MakeSendBuffer(pkt, PKT_C_GET_CLIENT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_SET_NICKNAME& pkt) { return MakeSendBuffer(pkt, PKT_C_SET_NICKNAME); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_C_CHAT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_BASE_INSTANTIATE_OBJECT& pkt) { return MakeSendBuffer(pkt, PKT_C_BASE_INSTANTIATE_OBJECT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_BASE_GET_OBJECT& pkt) { return MakeSendBuffer(pkt, PKT_C_BASE_GET_OBJECT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_BASE_SET_OBJECT_DATA& pkt) { return MakeSendBuffer(pkt, PKT_C_BASE_SET_OBJECT_DATA); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_BASE_SET_TRANSFORM& pkt) { return MakeSendBuffer(pkt, PKT_C_BASE_SET_TRANSFORM); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_BASE_SET_ANIMATION& pkt) { return MakeSendBuffer(pkt, PKT_C_BASE_SET_ANIMATION); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_BASE_SET_ANIMATION_ONCE& pkt) { return MakeSendBuffer(pkt, PKT_C_BASE_SET_ANIMATION_ONCE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_INTERACTION_GET_ITEMS& pkt) { return MakeSendBuffer(pkt, PKT_C_INTERACTION_GET_ITEMS); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_INTERACTION_SET_ITEM& pkt) { return MakeSendBuffer(pkt, PKT_C_INTERACTION_SET_ITEM); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_INTERACTION_REMOVE_ITEM& pkt) { return MakeSendBuffer(pkt, PKT_C_INTERACTION_REMOVE_ITEM); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_MYROOM_GET_ROOMINFO& pkt) { return MakeSendBuffer(pkt, PKT_C_MYROOM_GET_ROOMINFO); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_MYROOM_SET_ROOMINFO& pkt) { return MakeSendBuffer(pkt, PKT_C_MYROOM_SET_ROOMINFO); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_MYROOM_OTHER_ROOM_LIST& pkt) { return MakeSendBuffer(pkt, PKT_C_MYROOM_OTHER_ROOM_LIST); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_MYROOM_START_EDIT& pkt) { return MakeSendBuffer(pkt, PKT_C_MYROOM_START_EDIT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_MYROOM_END_EDIT& pkt) { return MakeSendBuffer(pkt, PKT_C_MYROOM_END_EDIT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_MYROOM_KICK& pkt) { return MakeSendBuffer(pkt, PKT_C_MYROOM_KICK); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_MYROOM_SHUTDOWN& pkt) { return MakeSendBuffer(pkt, PKT_C_MYROOM_SHUTDOWN); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OFFICE_GET_WAITING_LIST& pkt) { return MakeSendBuffer(pkt, PKT_C_OFFICE_GET_WAITING_LIST); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OFFICE_ACCEPT_WAIT& pkt) { return MakeSendBuffer(pkt, PKT_C_OFFICE_ACCEPT_WAIT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OFFICE_GET_HOST& pkt) { return MakeSendBuffer(pkt, PKT_C_OFFICE_GET_HOST); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OFFICE_BREAK& pkt) { return MakeSendBuffer(pkt, PKT_C_OFFICE_BREAK); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OFFICE_KICK& pkt) { return MakeSendBuffer(pkt, PKT_C_OFFICE_KICK); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OFFICE_GET_PERMISSION& pkt) { return MakeSendBuffer(pkt, PKT_C_OFFICE_GET_PERMISSION); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OFFICE_SET_PERMISSION& pkt) { return MakeSendBuffer(pkt, PKT_C_OFFICE_SET_PERMISSION); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OFFICE_SET_ROOM_INFO& pkt) { return MakeSendBuffer(pkt, PKT_C_OFFICE_SET_ROOM_INFO); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OFFICE_GET_ROOM_INFO& pkt) { return MakeSendBuffer(pkt, PKT_C_OFFICE_GET_ROOM_INFO); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OFFICE_VIDEO_STREAM& pkt) { return MakeSendBuffer(pkt, PKT_C_OFFICE_VIDEO_STREAM); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OFFICE_SHARE& pkt) { return MakeSendBuffer(pkt, PKT_C_OFFICE_SHARE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_MATCHING_START& pkt) { return MakeSendBuffer(pkt, PKT_C_MATCHING_START); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_MATCHING_GET_HOST& pkt) { return MakeSendBuffer(pkt, PKT_C_MATCHING_GET_HOST); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_MATCHING_DIE& pkt) { return MakeSendBuffer(pkt, PKT_C_MATCHING_DIE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OX_START& pkt) { return MakeSendBuffer(pkt, PKT_C_OX_START); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OX_GET_HOST& pkt) { return MakeSendBuffer(pkt, PKT_C_OX_GET_HOST); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_OX_DIE& pkt) { return MakeSendBuffer(pkt, PKT_C_OX_DIE); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, shared_ptr<GameSession>& session, unsigned char* buffer, int len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static shared_ptr<SendBuffer> MakeSendBuffer(T& pkt, unsigned short pktId)
	{
		const unsigned short dataSize = static_cast<unsigned short>(pkt.ByteSizeLong());
		const unsigned short packetSize = dataSize + sizeof(PacketHeader);

		shared_ptr<SendBuffer> sendBuffer = GSendBufferManager->Open(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		pkt.SerializeToArray(&header[1], dataSize);
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};