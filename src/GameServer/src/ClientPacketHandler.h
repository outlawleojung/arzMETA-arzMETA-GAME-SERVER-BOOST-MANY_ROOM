#pragma once

#include "./Util/pch.h"
#include "./Util/Protocols.h"

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
	PKT_C_SET_NICKNAME = 5,
	PKT_S_SET_NICKNAME = 6,
	PKT_S_SET_NICKNAME_NOTICE = 7,
	PKT_S_DISCONNECT = 8,
	PKT_C_OFFICE_GET_WAITING_LIST = 100,
	PKT_S_OFFICE_ADD_WAITING_CLIENT = 101,
	PKT_S_OFFICE_REMOVE_WAITING_CLIENT = 102,
	PKT_C_OFFICE_ACCEPT_WAIT = 103,
	PKT_S_OFFICE_ACCEPT_WAIT = 104,
	PKT_S_OFFICE_ACCEPT_WAIT_NOTICE = 105,
	PKT_C_OFFICE_GET_CLIENT = 106,
	PKT_S_OFFICE_ADD_CLIENT = 107,
	PKT_S_OFFICE_REMOVE_CLIENT = 108,
	PKT_C_OFFICE_GET_HOST = 109,
	PKT_S_OFFICE_GET_HOST = 110,
	PKT_C_OFFICE_BREAK = 111,
	PKT_S_OFFICE_BREAK = 112,
	PKT_C_OFFICE_KICK = 113,
	PKT_S_OFFICE_KICK = 114,
	PKT_C_OFFICE_GET_PERMISSION = 115,
	PKT_S_OFFICE_GET_PERMISSION = 116,
	PKT_C_OFFICE_SET_PERMISSION = 117,
	PKT_S_OFFICE_SET_PERMISSION = 118,
	PKT_S_OFFICE_SET_PERMISSION_NOTICE = 119,
	PKT_C_OFFICE_SET_ROOM_INFO = 120,
	PKT_S_OFFICE_SET_ROOM_INFO = 121,
	PKT_C_OFFICE_GET_ROOM_INFO = 122,
	PKT_S_OFFICE_GET_ROOM_INFO = 123,
	PKT_C_OFFICE_VIDEO_STREAM = 124,
	PKT_S_OFFICE_VIDEO_STREAM = 125,
	PKT_C_MYROOM_GET_ROOMINFO = 200,
	PKT_S_MYROOM_GET_ROOMINFO = 201,
	PKT_C_MYROOM_SET_ROOMINFO = 202,
	PKT_S_MYROOM_SET_ROOMINFO = 203,
	PKT_C_MYROOM_OTHER_ROOM_LIST = 204,
	PKT_S_MYROOM_OTHER_ROOM_LIST = 205,
	PKT_C_BASE_SET_SCENE = 10100,
	PKT_S_BASE_SET_SCENE = 10101,
	PKT_C_BASE_INSTANTIATE_OBJECT = 10102,
	PKT_S_BASE_INSTANTIATE_OBJECT = 10103,
	PKT_C_BASE_GET_OBJECT = 10104,
	PKT_S_BASE_ADD_OBJECT = 10105,
	PKT_S_BASE_REMOVE_OBJECT = 10106,
	PKT_C_BASE_SET_OBJECT_DATA = 10107,
	PKT_S_BASE_SET_OBJECT_DATA = 10108,
	PKT_S_BASE_SET_OBJECT_DATA_NOTICE = 10109,
	PKT_C_BASE_SET_TRANSFORM = 10110,
	PKT_S_BASE_SET_TRANSFORM = 10111,
	PKT_C_BASE_SET_ANIMATION = 10112,
	PKT_S_BASE_SET_ANIMATION = 10113,
	PKT_C_BASE_SET_ANIMATION_ONCE = 10114,
	PKT_S_BASE_SET_ANIMATION_ONCE = 10115,
	PKT_C_INTERACTION_GET_ITEMS = 10200,
	PKT_S_INTERACTION_GET_ITEMS = 10201,
	PKT_C_INTERACTION_SET_ITEM = 10202,
	PKT_S_INTERACTION_SET_ITEM = 10203,
	PKT_S_INTERACTION_SET_ITEM_NOTICE = 10204,
	PKT_C_INTERACTION_REMOVE_ITEM = 10205,
	PKT_S_INTERACTION_REMOVE_ITEM = 10206,
	PKT_S_INTERACTION_REMOVE_ITEM_NOTICE = 10207,
};

bool Handle_INVALID(shared_ptr<GameSession>& session, unsigned char* buffer, int len);
bool Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt);
bool Handle_C_REENTER(shared_ptr<GameSession>& session, Protocol::C_REENTER& pkt);
bool Handle_C_LEAVE(shared_ptr<GameSession>& session, Protocol::C_LEAVE& pkt);
bool Handle_C_SET_NICKNAME(shared_ptr<GameSession>& session, Protocol::C_SET_NICKNAME& pkt);
bool Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_WAITING_LIST& pkt);
bool Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_ACCEPT_WAIT& pkt);
bool Handle_C_OFFICE_GET_CLIENT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_CLIENT& pkt);
bool Handle_C_OFFICE_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_HOST& pkt);
bool Handle_C_OFFICE_BREAK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_BREAK& pkt);
bool Handle_C_OFFICE_KICK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_KICK& pkt);
bool Handle_C_OFFICE_GET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_PERMISSION& pkt);
bool Handle_C_OFFICE_SET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_PERMISSION& pkt);
bool Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_ROOM_INFO& pkt);
bool Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_ROOM_INFO& pkt);
bool Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<GameSession>& session, Protocol::C_OFFICE_VIDEO_STREAM& pkt);
bool Handle_C_MYROOM_GET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_GET_ROOMINFO& pkt);
bool Handle_C_MYROOM_SET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_SET_ROOMINFO& pkt);
bool Handle_C_MYROOM_OTHER_ROOM_LIST(shared_ptr<GameSession>& session, Protocol::C_MYROOM_OTHER_ROOM_LIST& pkt);
bool Handle_C_BASE_SET_SCENE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_SCENE& pkt);
bool Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt);
bool Handle_C_BASE_GET_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_GET_OBJECT& pkt);
bool Handle_C_BASE_SET_OBJECT_DATA(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_OBJECT_DATA& pkt);
bool Handle_C_BASE_SET_TRANSFORM(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_TRANSFORM& pkt);
bool Handle_C_BASE_SET_ANIMATION(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION& pkt);
bool Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt);
bool Handle_C_INTERACTION_GET_ITEMS(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_GET_ITEMS& pkt);
bool Handle_C_INTERACTION_SET_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_SET_ITEM& pkt);
bool Handle_C_INTERACTION_REMOVE_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_REMOVE_ITEM& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_C_ENTER] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_ENTER>(Handle_C_ENTER, session, buffer, len); };
		GPacketHandler[PKT_C_REENTER] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_REENTER>(Handle_C_REENTER, session, buffer, len); };
		GPacketHandler[PKT_C_LEAVE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_LEAVE>(Handle_C_LEAVE, session, buffer, len); };
		GPacketHandler[PKT_C_SET_NICKNAME] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_SET_NICKNAME>(Handle_C_SET_NICKNAME, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_GET_WAITING_LIST] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_OFFICE_GET_WAITING_LIST>(Handle_C_OFFICE_GET_WAITING_LIST, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_ACCEPT_WAIT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_OFFICE_ACCEPT_WAIT>(Handle_C_OFFICE_ACCEPT_WAIT, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_GET_CLIENT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_OFFICE_GET_CLIENT>(Handle_C_OFFICE_GET_CLIENT, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_GET_HOST] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_OFFICE_GET_HOST>(Handle_C_OFFICE_GET_HOST, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_BREAK] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_OFFICE_BREAK>(Handle_C_OFFICE_BREAK, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_KICK] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_OFFICE_KICK>(Handle_C_OFFICE_KICK, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_GET_PERMISSION] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_OFFICE_GET_PERMISSION>(Handle_C_OFFICE_GET_PERMISSION, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_SET_PERMISSION] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_OFFICE_SET_PERMISSION>(Handle_C_OFFICE_SET_PERMISSION, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_SET_ROOM_INFO] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_OFFICE_SET_ROOM_INFO>(Handle_C_OFFICE_SET_ROOM_INFO, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_GET_ROOM_INFO] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_OFFICE_GET_ROOM_INFO>(Handle_C_OFFICE_GET_ROOM_INFO, session, buffer, len); };
		GPacketHandler[PKT_C_OFFICE_VIDEO_STREAM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_OFFICE_VIDEO_STREAM>(Handle_C_OFFICE_VIDEO_STREAM, session, buffer, len); };
		GPacketHandler[PKT_C_MYROOM_GET_ROOMINFO] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_MYROOM_GET_ROOMINFO>(Handle_C_MYROOM_GET_ROOMINFO, session, buffer, len); };
		GPacketHandler[PKT_C_MYROOM_SET_ROOMINFO] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_MYROOM_SET_ROOMINFO>(Handle_C_MYROOM_SET_ROOMINFO, session, buffer, len); };
		GPacketHandler[PKT_C_MYROOM_OTHER_ROOM_LIST] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_MYROOM_OTHER_ROOM_LIST>(Handle_C_MYROOM_OTHER_ROOM_LIST, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_SET_SCENE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_BASE_SET_SCENE>(Handle_C_BASE_SET_SCENE, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_INSTANTIATE_OBJECT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_BASE_INSTANTIATE_OBJECT>(Handle_C_BASE_INSTANTIATE_OBJECT, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_GET_OBJECT] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_BASE_GET_OBJECT>(Handle_C_BASE_GET_OBJECT, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_SET_OBJECT_DATA] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_BASE_SET_OBJECT_DATA>(Handle_C_BASE_SET_OBJECT_DATA, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_SET_TRANSFORM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_BASE_SET_TRANSFORM>(Handle_C_BASE_SET_TRANSFORM, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_SET_ANIMATION] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_BASE_SET_ANIMATION>(Handle_C_BASE_SET_ANIMATION, session, buffer, len); };
		GPacketHandler[PKT_C_BASE_SET_ANIMATION_ONCE] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_BASE_SET_ANIMATION_ONCE>(Handle_C_BASE_SET_ANIMATION_ONCE, session, buffer, len); };
		GPacketHandler[PKT_C_INTERACTION_GET_ITEMS] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_INTERACTION_GET_ITEMS>(Handle_C_INTERACTION_GET_ITEMS, session, buffer, len); };
		GPacketHandler[PKT_C_INTERACTION_SET_ITEM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_INTERACTION_SET_ITEM>(Handle_C_INTERACTION_SET_ITEM, session, buffer, len); };
		GPacketHandler[PKT_C_INTERACTION_REMOVE_ITEM] = [](shared_ptr<GameSession>& session, unsigned char* buffer, int len) { return HandlePacket<Protocol::C_INTERACTION_REMOVE_ITEM>(Handle_C_INTERACTION_REMOVE_ITEM, session, buffer, len); };
	}

	static bool HandlePacket(shared_ptr<GameSession>& session, unsigned char* buffer, int len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_ENTER& pkt) { return MakeSendBuffer(pkt, PKT_S_ENTER); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_REENTER& pkt) { return MakeSendBuffer(pkt, PKT_S_REENTER); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_SET_NICKNAME& pkt) { return MakeSendBuffer(pkt, PKT_S_SET_NICKNAME); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_SET_NICKNAME_NOTICE& pkt) { return MakeSendBuffer(pkt, PKT_S_SET_NICKNAME_NOTICE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_DISCONNECT& pkt) { return MakeSendBuffer(pkt, PKT_S_DISCONNECT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_ADD_WAITING_CLIENT& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_ADD_WAITING_CLIENT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_REMOVE_WAITING_CLIENT& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_REMOVE_WAITING_CLIENT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_ACCEPT_WAIT& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_ACCEPT_WAIT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_ACCEPT_WAIT_NOTICE& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_ACCEPT_WAIT_NOTICE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_ADD_CLIENT& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_ADD_CLIENT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_REMOVE_CLIENT& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_REMOVE_CLIENT); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_GET_HOST& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_GET_HOST); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_BREAK& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_BREAK); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_KICK& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_KICK); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_GET_PERMISSION& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_GET_PERMISSION); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_SET_PERMISSION& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_SET_PERMISSION); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_SET_PERMISSION_NOTICE& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_SET_PERMISSION_NOTICE); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_SET_ROOM_INFO& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_SET_ROOM_INFO); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_GET_ROOM_INFO& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_GET_ROOM_INFO); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_OFFICE_VIDEO_STREAM& pkt) { return MakeSendBuffer(pkt, PKT_S_OFFICE_VIDEO_STREAM); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MYROOM_GET_ROOMINFO& pkt) { return MakeSendBuffer(pkt, PKT_S_MYROOM_GET_ROOMINFO); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MYROOM_SET_ROOMINFO& pkt) { return MakeSendBuffer(pkt, PKT_S_MYROOM_SET_ROOMINFO); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_MYROOM_OTHER_ROOM_LIST& pkt) { return MakeSendBuffer(pkt, PKT_S_MYROOM_OTHER_ROOM_LIST); }
	static shared_ptr<SendBuffer> MakeSendBuffer(Protocol::S_BASE_SET_SCENE& pkt) { return MakeSendBuffer(pkt, PKT_S_BASE_SET_SCENE); }
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