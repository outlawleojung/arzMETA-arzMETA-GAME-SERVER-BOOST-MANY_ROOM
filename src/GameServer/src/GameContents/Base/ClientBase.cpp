#include "ClientBase.h"
#include "RoomBase.h"
#include "../../Session/GameSession.h"
#include "../../ClientPacketHandler.h"

void ClientBase::Leave(string code)
{
	if (state == ClientState::LEAVING)
		return;

	state = ClientState::LEAVING;

	enteredRoom->DoAsync(&RoomBase::Leave, static_pointer_cast<ClientBase>(shared_from_this()));
	enteredRoom = nullptr;

	Protocol::S_DISCONNECT disconnect;
	disconnect.set_code(code);
	Send(ClientPacketHandler::MakeSendBuffer(disconnect));

	session->Disconnect();
	session->owner = nullptr;
	session = nullptr;
}

void ClientBase::Send(shared_ptr<SendBuffer> sendBuffer)
{
	session->Send(sendBuffer);
}

void ClientBase::ReEnter(shared_ptr<GameSession> session)
{
	Protocol::S_REENTER res;

	if (state == ClientState::LEAVING)
	{
		res.set_success(false);
		session->Send(ClientPacketHandler::MakeSendBuffer(res));
		return;
	}

	this->session = session;

	res.set_success(true);
	session->Send(ClientPacketHandler::MakeSendBuffer(res));
}

void ClientBase::OnDisconnected()
{
	if (state == ClientState::LEAVING)
		return;

	this->DoTimer(enteredRoom->disconnectedSessionWaitTime, [this]() {
		if (session->IsConnected())
			return;

		DoAsync(&ClientBase::Leave, string("Disconnected"));
		});
}
