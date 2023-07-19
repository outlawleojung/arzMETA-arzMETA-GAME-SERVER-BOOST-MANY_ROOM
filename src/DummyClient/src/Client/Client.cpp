#include "Client.h"
#include "../Session/GameSession.h"
#include "../Protocols.h"
#include "../PacketManager.h"

void Client::Send(shared_ptr<SendBuffer> sendBuffer)
{
	session->Send(sendBuffer);
}

void Client::OnDisconnected()
{
	clients.erase(clientId);
	session = nullptr;
}

void Client::StartMove()
{
	Protocol::C_BASE_SET_TRANSFORM setTransform;
	auto position = setTransform.mutable_position();

	//randomly add value to position_x, y, z, from -1 to 1
	position_x += (rand() % 3) - 1;
	position_z += (rand() % 3) - 1;

	position->set_x(position_x);
	position->set_y(position_y);
	position->set_z(position_z);

	auto rotation = setTransform.mutable_rotation();
	rotation->set_x(0);
	rotation->set_y(0);
	rotation->set_z(0);

	setTransform.set_objectid(objectId);

	Send(PacketManager::MakeSendBuffer(setTransform));

	DoTimer(200, &Client::StartMove);
}
