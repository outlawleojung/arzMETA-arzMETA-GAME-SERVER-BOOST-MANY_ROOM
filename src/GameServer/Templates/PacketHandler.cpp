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

{%- for pkt in parser.recv_pkt %}
	{%- if (pkt.name != 'C_ENTER') %}
bool Handle_{{pkt.name}}(shared_ptr<GameSession>& session, Protocol::{{pkt.name}}& pkt)
{
	if (session->enteredRoom->state != RoomState::Running)
		return true;
	
	session->enteredRoom->Handle_{{pkt.name}}(session, pkt);
	
	return true;
}
	{%- endif %}
{%- endfor %}
