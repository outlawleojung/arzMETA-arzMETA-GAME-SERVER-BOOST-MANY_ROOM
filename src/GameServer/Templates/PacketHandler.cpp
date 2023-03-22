#include "ClientPacketHandler.h"
#include "./Session/GameSession.h"
#include "GameContents/ClientManager.h"
#include "GameContents/Base/ClientBase.h"
#include "GameContents/Base/RoomBase.h"
#include "GameContents/RoomManager.h"

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
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
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
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		return false;
	}
	client->DoAsync(&ClientBase::ReEnter, session);
	
	return true;
}

{% for pkt in parser.recv_pkt %}
	{%- if (pkt.name != 'C_ENTER') and (pkt.name != 'C_REENTER') %}
bool Handle_{{pkt.name}}(shared_ptr<GameSession>& session, Protocol::{{pkt.name}}& pkt)
{
	if(session->owner == nullptr)
		return false;

	if (session->owner->enteredRoom->state != RoomState::Running)
		return true;
	
	session->owner->enteredRoom->Handle_{{pkt.name}}(session->owner, pkt);
	
	return true;
}
	{%- endif %}
{%- endfor %}
