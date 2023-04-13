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
	PKT_S_DISCONNECT = 11,
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
bool Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt);
bool Handle_C_REENTER(shared_ptr<GameSession>& session, Protocol::C_REENTER& pkt);
bool Handle_C_LEAVE(shared_ptr<GameSession>& session, Protocol::C_LEAVE& pkt);
bool Handle_C_GET_CLIENT(shared_ptr<GameSession>& session, Protocol::C_GET_CLIENT& pkt);
bool Handle_C_SET_NICKNAME(shared_ptr<GameSession>& session, Protocol::C_SET_NICKNAME& pkt);
bool Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt);
bool Handle_C_BASE_GET_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_GET_OBJECT& pkt);
bool Handle_C_BASE_SET_OBJECT_DATA(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_OBJECT_DATA& pkt);
bool Handle_C_BASE_SET_TRANSFORM(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_TRANSFORM& pkt);
bool Handle_C_BASE_SET_ANIMATION(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION& pkt);
bool Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt);
bool Handle_C_INTERACTION_GET_ITEMS(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_GET_ITEMS& pkt);
bool Handle_C_INTERACTION_SET_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_SET_ITEM& pkt);
bool Handle_C_INTERACTION_REMOVE_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_REMOVE_ITEM& pkt);
bool Handle_C_MYROOM_GET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_GET_ROOMINFO& pkt);
bool Handle_C_MYROOM_SET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_SET_ROOMINFO& pkt);
bool Handle_C_MYROOM_OTHER_ROOM_LIST(shared_ptr<GameSession>& session, Protocol::C_MYROOM_OTHER_ROOM_LIST& pkt);
bool Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_WAITING_LIST& pkt);
bool Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_ACCEPT_WAIT& pkt);
bool Handle_C_OFFICE_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_HOST& pkt);
bool Handle_C_OFFICE_BREAK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_BREAK& pkt);
bool Handle_C_OFFICE_KICK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_KICK& pkt);
bool Handle_C_OFFICE_GET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_PERMISSION& pkt);
bool Handle_C_OFFICE_SET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_PERMISSION& pkt);
bool Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_ROOM_INFO& pkt);
bool Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_ROOM_INFO& pkt);
bool Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<GameSession>& session, Protocol::C_OFFICE_VIDEO_STREAM& pkt);
bool Handle_C_OFFICE_SHARE(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SHARE& pkt);
bool Handle_C_MATCHING_START(shared_ptr<GameSession>& session, Protocol::C_MATCHING_START& pkt);
bool Handle_C_MATCHING_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_MATCHING_GET_HOST& pkt);
bool Handle_C_MATCHING_DIE(shared_ptr<GameSession>& session, Protocol::C_MATCHING_DIE& pkt);
bool Handle_C_OX_START(shared_ptr<GameSession>& session, Protocol::C_OX_START& pkt);
bool Handle_C_OX_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_OX_GET_HOST& pkt);
bool Handle_C_OX_DIE(shared_ptr<GameSession>& session, Protocol::C_OX_DIE& pkt);

class PacketManager
{
public:
	static void Init()
	{
		for (int i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_C_ENTER] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_ENTER > (Handle_C_ENTER, session, buffer, len); };
		GPacketHandler[PKT_C_REENTER] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_REENTER > (Handle_C_REENTER, session, buffer, len); };
		GPacketHandler[PKT_C_LEAVE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_LEAVE > (Handle_C_LEAVE, session, buffer, len); };
		GPacketHandler[PKT_C_GET_CLIENT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_GET_CLIENT > (Handle_C_GET_CLIENT, session, buffer, len); };
		GPacketHandler[PKT_C_SET_NICKNAME] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_SET_NICKNAME > (Handle_C_SET_NICKNAME, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_INSTANTIATE_OBJECT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_BASE_INSTANTIATE_OBJECT > (Handle_C_BASE_INSTANTIATE_OBJECT, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_GET_OBJECT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_BASE_GET_OBJECT > (Handle_C_BASE_GET_OBJECT, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_SET_OBJECT_DATA] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_BASE_SET_OBJECT_DATA > (Handle_C_BASE_SET_OBJECT_DATA, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_SET_TRANSFORM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_BASE_SET_TRANSFORM > (Handle_C_BASE_SET_TRANSFORM, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_SET_ANIMATION] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_BASE_SET_ANIMATION > (Handle_C_BASE_SET_ANIMATION, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_SET_ANIMATION_ONCE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_BASE_SET_ANIMATION_ONCE > (Handle_C_BASE_SET_ANIMATION_ONCE, session, buffer, len); };
		GPacketHandler[PKT_C_INTERACTION_GET_ITEMS] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_INTERACTION_GET_ITEMS > (Handle_C_INTERACTION_GET_ITEMS, session, buffer, len); };
		GPacketHandler[PKT_C_INTERACTION_SET_ITEM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_INTERACTION_SET_ITEM > (Handle_C_INTERACTION_SET_ITEM, session, buffer, len); };
		GPacketHandler[PKT_C_INTERACTION_REMOVE_ITEM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_INTERACTION_REMOVE_ITEM > (Handle_C_INTERACTION_REMOVE_ITEM, session, buffer, len); };
		GPacketHandler[PKT_C_MYROOM_GET_ROOMINFO] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_MYROOM_GET_ROOMINFO > (Handle_C_MYROOM_GET_ROOMINFO, session, buffer, len); };
		GPacketHandler[PKT_C_MYROOM_SET_ROOMINFO] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_MYROOM_SET_ROOMINFO > (Handle_C_MYROOM_SET_ROOMINFO, session, buffer, len); };
		GPacketHandler[PKT_C_MYROOM_OTHER_ROOM_LIST] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_MYROOM_OTHER_ROOM_LIST > (Handle_C_MYROOM_OTHER_ROOM_LIST, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_GET_WAITING_LIST] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OFFICE_GET_WAITING_LIST > (Handle_C_OFFICE_GET_WAITING_LIST, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_ACCEPT_WAIT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OFFICE_ACCEPT_WAIT > (Handle_C_OFFICE_ACCEPT_WAIT, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_GET_HOST] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OFFICE_GET_HOST > (Handle_C_OFFICE_GET_HOST, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_BREAK] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OFFICE_BREAK > (Handle_C_OFFICE_BREAK, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_KICK] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OFFICE_KICK > (Handle_C_OFFICE_KICK, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_GET_PERMISSION] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OFFICE_GET_PERMISSION > (Handle_C_OFFICE_GET_PERMISSION, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_SET_PERMISSION] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OFFICE_SET_PERMISSION > (Handle_C_OFFICE_SET_PERMISSION, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_SET_ROOM_INFO] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OFFICE_SET_ROOM_INFO > (Handle_C_OFFICE_SET_ROOM_INFO, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_GET_ROOM_INFO] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OFFICE_GET_ROOM_INFO > (Handle_C_OFFICE_GET_ROOM_INFO, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_VIDEO_STREAM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OFFICE_VIDEO_STREAM > (Handle_C_OFFICE_VIDEO_STREAM, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_SHARE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OFFICE_SHARE > (Handle_C_OFFICE_SHARE, session, buffer, len); };
		GPacketHandler[PKT_C_MATCHING_START] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_MATCHING_START > (Handle_C_MATCHING_START, session, buffer, len); };
		GPacketHandler[PKT_C_MATCHING_GET_HOST] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_MATCHING_GET_HOST > (Handle_C_MATCHING_GET_HOST, session, buffer, len); };
		GPacketHandler[PKT_C_MATCHING_DIE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_MATCHING_DIE > (Handle_C_MATCHING_DIE, session, buffer, len); };
		GPacketHandler[PKT_C_OX_START] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OX_START > (Handle_C_OX_START, session, buffer, len); };
		GPacketHandler[PKT_C_OX_GET_HOST] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OX_GET_HOST > (Handle_C_OX_GET_HOST, session, buffer, len); };
		GPacketHandler[PKT_C_OX_DIE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket < Protocol::C_OX_DIE > (Handle_C_OX_DIE, session, buffer, len); };
	}

	static bool HandlePacket(shared_ptr<GameSession>& session, unsigned char* buffer, int len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_ENTER& pkt) { return MakeSendBuffer(pkt, PKT_S_ENTER); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_REENTER& pkt) { return MakeSendBuffer(pkt, PKT_S_REENTER); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_ADD_CLIENT& pkt) { return MakeSendBuffer(pkt, PKT_S_ADD_CLIENT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_REMOVE_CLIENT& pkt) { return MakeSendBuffer(pkt, PKT_S_REMOVE_CLIENT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_SET_NICKNAME& pkt) { return MakeSendBuffer(pkt, PKT_S_SET_NICKNAME); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_SET_NICKNAME_NOTICE& pkt) { return MakeSendBuffer(pkt, PKT_S_SET_NICKNAME_NOTICE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_DISCONNECT& pkt) { return MakeSendBuffer(pkt, PKT_S_DISCONNECT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_BASE_INSTANTIATE_OBJECT& pkt) { return MakeSendBuffer(pkt, PKT_S_BASE_INSTANTIATE_OBJECT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_BASE_ADD_OBJECT& pkt) { return MakeSendBuffer(pkt, PKT_S_BASE_ADD_OBJECT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_BASE_REMOVE_OBJECT& pkt) { return MakeSendBuffer(pkt, PKT_S_BASE_REMOVE_OBJECT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_BASE_SET_OBJECT_DATA& pkt) { return MakeSendBuffer(pkt, PKT_S_BASE_SET_OBJECT_DATA); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_BASE_SET_OBJECT_DATA_NOTICE& pkt) { return MakeSendBuffer(pkt, PKT_S_BASE_SET_OBJECT_DATA_NOTICE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_BASE_SET_TRANSFORM& pkt) { return MakeSendBuffer(pkt, PKT_S_BASE_SET_TRANSFORM); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_BASE_SET_ANIMATION& pkt) { return MakeSendBuffer(pkt, PKT_S_BASE_SET_ANIMATION); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_BASE_SET_ANIMATION_ONCE& pkt) { return MakeSendBuffer(pkt, PKT_S_BASE_SET_ANIMATION_ONCE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_INTERACTION_GET_ITEMS& pkt) { return MakeSendBuffer(pkt, PKT_S_INTERACTION_GET_ITEMS); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_INTERACTION_SET_ITEM& pkt) { return MakeSendBuffer(pkt, PKT_S_INTERACTION_SET_ITEM); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_INTERACTION_SET_ITEM_NOTICE& pkt) { return MakeSendBuffer(pkt, PKT_S_INTERACTION_SET_ITEM_NOTICE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_INTERACTION_REMOVE_ITEM& pkt) { return MakeSendBuffer(pkt, PKT_S_INTERACTION_REMOVE_ITEM); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_INTERACTION_REMOVE_ITEM_NOTICE& pkt) { return MakeSendBuffer(pkt, PKT_S_INTERACTION_REMOVE_ITEM_NOTICE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MYROOM_GET_ROOMINFO& pkt) { return MakeSendBuffer(pkt, PKT_S_MYROOM_GET_ROOMINFO); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MYROOM_SET_ROOMINFO& pkt) { return MakeSendBuffer(pkt, PKT_S_MYROOM_SET_ROOMINFO); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MYROOM_OTHER_ROOM_LIST& pkt) { return MakeSendBuffer(pkt, PKT_S_MYROOM_OTHER_ROOM_LIST); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_ADD_WAITING_CLIENT& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_ADD_WAITING_CLIENT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_REMOVE_WAITING_CLIENT& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_REMOVE_WAITING_CLIENT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_ACCEPT_WAIT& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_ACCEPT_WAIT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_ACCEPT_WAIT_NOTICE& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_ACCEPT_WAIT_NOTICE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_GET_HOST& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_GET_HOST); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_BREAK& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_BREAK); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_KICK& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_KICK); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_GET_PERMISSION& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_GET_PERMISSION); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_SET_PERMISSION& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_SET_PERMISSION); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_SET_PERMISSION_NOTICE& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_SET_PERMISSION_NOTICE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_SET_ROOM_INFO& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_SET_ROOM_INFO); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_GET_ROOM_INFO& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_GET_ROOM_INFO); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_VIDEO_STREAM& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_VIDEO_STREAM); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_SHARE& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_SHARE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MATCHING_START& pkt) { return MakeSendBuffer(pkt, PKT_S_MATCHING_START); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MATCHING_FINISH& pkt) { return MakeSendBuffer(pkt, PKT_S_MATCHING_FINISH); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MATCHING_HOST& pkt) { return MakeSendBuffer(pkt, PKT_S_MATCHING_HOST); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MATCHING_ROUND_START& pkt) { return MakeSendBuffer(pkt, PKT_S_MATCHING_ROUND_START); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MATCHING_ROUND_FINISH& pkt) { return MakeSendBuffer(pkt, PKT_S_MATCHING_ROUND_FINISH); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MATCHING_TILES& pkt) { return MakeSendBuffer(pkt, PKT_S_MATCHING_TILES); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MATCHING_HINT& pkt) { return MakeSendBuffer(pkt, PKT_S_MATCHING_HINT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MATCHING_PROBLEM& pkt) { return MakeSendBuffer(pkt, PKT_S_MATCHING_PROBLEM); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MATCHING_DESTROY& pkt) { return MakeSendBuffer(pkt, PKT_S_MATCHING_DESTROY); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MATCHING_QUIZ_DISAPPEAR& pkt) { return MakeSendBuffer(pkt, PKT_S_MATCHING_QUIZ_DISAPPEAR); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OX_START& pkt) { return MakeSendBuffer(pkt, PKT_S_OX_START); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OX_FINISH& pkt) { return MakeSendBuffer(pkt, PKT_S_OX_FINISH); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OX_HOST& pkt) { return MakeSendBuffer(pkt, PKT_S_OX_HOST); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OX_ROUND_START& pkt) { return MakeSendBuffer(pkt, PKT_S_OX_ROUND_START); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OX_ROUND_FINISH& pkt) { return MakeSendBuffer(pkt, PKT_S_OX_ROUND_FINISH); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OX_QUIZ& pkt) { return MakeSendBuffer(pkt, PKT_S_OX_QUIZ); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OX_DESTROY& pkt) { return MakeSendBuffer(pkt, PKT_S_OX_DESTROY); }

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