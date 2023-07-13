#pragma once

#include "../Game/GameRoom.h"

class MyRoomRoom : public GameRoom
{
public:
	virtual void Init() override;

	virtual void Handle_C_MYROOM_GET_ROOMINFO(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_GET_ROOMINFO& pkt) override;
	virtual void Handle_C_MYROOM_START_EDIT(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_START_EDIT& pkt) override;
	virtual void Handle_C_MYROOM_END_EDIT(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_END_EDIT& pkt) override;
	virtual void Handle_C_MYROOM_KICK(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_KICK& pkt) override;
	virtual void Handle_C_MYROOM_SHUTDOWN(shared_ptr<ClientBase>& client, Protocol::C_MYROOM_SHUTDOWN& pkt) override;

	virtual void Handle_C_SET_NICKNAME(shared_ptr<ClientBase>& client, Protocol::C_SET_NICKNAME& pkt) override;

	virtual void Handle_C_BASE_SET_OBJECT_DATA(shared_ptr<ClientBase>& session, Protocol::C_BASE_SET_OBJECT_DATA& pkt) override;

	virtual pair<bool, string> HandleEnter(const Protocol::C_ENTER& pkt) override;

	void GetRoomInfo(shared_ptr<ClientBase> client);
	void HandleStartEdit(shared_ptr<ClientBase> client);
	void HandleEndEdit(shared_ptr<ClientBase> client, bool isChanged);
	void Kick(shared_ptr<ClientBase> client, string kickedClientId);
	void HandleShutdown(shared_ptr<ClientBase> client, bool isShutdown);

	void UpdateOnwerNickname(string nickname);
	void UpdateOnwerAvatarInfo(string objectData);

	string ownerId;
	string ownerNickname;
	nlohmann::json ownerAvatarInfo;
	string myroomInfo;
	bool isShutdown;
};