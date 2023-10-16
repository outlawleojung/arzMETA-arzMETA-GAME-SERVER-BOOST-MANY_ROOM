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

	//randomly choose to move or stop
	if (rand() % 2 == 0)
	{
		if (isMoving)
		{
			isMoving = false;
			Protocol::C_BASE_SET_ANIMATION setAnimation;
			setAnimation.set_objectid(objectId);
			setAnimation.set_animationid("Avatar");
			setAnimation.set_animation("M9AzAAIFA1QKCA0B");
			Send(PacketManager::MakeSendBuffer(setAnimation));
		}

		DoTimer(200, &Client::StartMove);
	}
	else
	{
		if (!isMoving)
		{
			isMoving = true;
			Protocol::C_BASE_SET_ANIMATION setAnimation;
			setAnimation.set_objectid(objectId);
			setAnimation.set_animation("M9MzAAIFA1QKCA0B");
			Send(PacketManager::MakeSendBuffer(setAnimation));
		}

		position_x += (rand() % 3) - 1;
		position_z += (rand() % 3) - 1;

		if(position_x > start_x + cap)
			position_x = start_x + cap;
		else if(position_x < start_x - cap)
			position_x = start_x - cap;

		if (position_z > start_z + cap)
			position_z = start_z + cap;
		else if (position_z < start_z - cap)
			position_z = start_z - cap;

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
}
