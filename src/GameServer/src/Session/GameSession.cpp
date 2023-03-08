#include "GameSession.h"

#include "../Util/pch.h"

#include "../Room/RoomBase.h"

#include "../ClientPacketHandler.h"

bool GameSession::Disconnect(string code)
{
	auto sessionState = state.exchange(SessionState::DESTRUCTING);
	if (sessionState == SessionState::DESTRUCTING)
		return false; //already destructing;

	Protocol::S_DISCONNECT disconnect;
	disconnect.set_code(code);
	Send(ClientPacketHandler::MakeSendBuffer(disconnect));

	Session::Disconnect();

	return true;
}

void GameSession::OnConnected()
{
}

void GameSession::OnDisconnected()
{
	state.exchange(SessionState::DESTRUCTING);

	/*SessionState normal = SessionState::NORMAL;
	auto isNormal = state.compare_exchange_strong(normal, SessionState::DESTRUCTING);
	if (!isNormal)
		return;*/

	GLogManager->Log("Session disconnected :		", clientId);

	if (enteredRoom == nullptr)
		return;

	enteredRoom->DoAsync(&RoomBase::Leave, static_pointer_cast<GameSession>(shared_from_this()));
	enteredRoom = nullptr;
}

void GameSession::OnRecvPacket(unsigned char* buffer, int len)
{
	auto session= static_pointer_cast<GameSession>(shared_from_this());
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	bool result = ClientPacketHandler::HandlePacket(session, buffer, len);
	if (!result)
		Session::Disconnect();
}
