#include "GameSession.h"

#include "../pch.h"
#include "../PacketManager.h"
#include "../Contents/Base/ClientBase.h"

void GameSession::OnConnected()
{
}

void GameSession::OnDisconnected()
{
	if (owner == nullptr)
		return;

	owner->DoAsync(&ClientBase::OnDisconnected);
}

void GameSession::OnRecvPacket(unsigned char* buffer, int len)
{
	auto session= static_pointer_cast<GameSession>(shared_from_this());
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	bool result = PacketManager::HandlePacket(session, buffer, len);
	if (!result)
		Session::Disconnect();
}
