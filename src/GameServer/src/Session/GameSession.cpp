#include "GameSession.h"

#include "../Util/pch.h"
#include "../ClientPacketHandler.h"
#include "../GameContents/Base/ClientBase.h"

void GameSession::OnConnected()
{
}

void GameSession::OnDisconnected()
{
	owner->DoAsync(&ClientBase::OnDisconnected);
}

void GameSession::OnRecvPacket(unsigned char* buffer, int len)
{
	auto session= static_pointer_cast<GameSession>(shared_from_this());
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	bool result = ClientPacketHandler::HandlePacket(session, buffer, len);
	if (!result)
		Session::Disconnect();
}
