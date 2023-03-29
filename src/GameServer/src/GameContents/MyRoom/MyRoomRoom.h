#pragma once

#include "../Game/GameRoom.h"

class MyRoomRoom : public GameRoom
{
public:
	MyRoomRoom(vector<string> sceneIds);

	virtual void Init() override;

	virtual void Handle_C_MYROOM_GET_ROOMINFO(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_GET_ROOMINFO& pkt);
	virtual void Handle_C_MYROOM_SET_ROOMINFO(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_SET_ROOMINFO& pkt);
	virtual void Handle_C_MYROOM_OTHER_ROOM_LIST(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_OTHER_ROOM_LIST& pkt);

	string ownerId;
	string roomInfo;
};