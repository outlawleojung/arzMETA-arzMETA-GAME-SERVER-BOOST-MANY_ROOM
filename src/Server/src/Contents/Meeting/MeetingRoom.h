#pragma once

#include "../Game/GameRoom.h"

#include <nlohmann/json.hpp>

class MeetingClient;

enum class MeetingRoomUserType : int
{
	Host = 1,
	Guest = 3
};

struct MeetingRoomUserData
{
public:
	MeetingRoomUserType type;
	bool screenPermission;
	bool chatPermission;
	bool voicePermission;
	bool videoPermission;

	bool operator==(const MeetingRoomUserData& other) const {
		return type == other.type
			&& screenPermission == other.screenPermission
			&& chatPermission == other.chatPermission
			&& voicePermission == other.voicePermission
			&& videoPermission == other.videoPermission;
	}

	bool operator!=(const MeetingRoomUserData& other) const {
		return !(*this == other);
	}
};

class MeetingRoom : public GameRoom
{
public:
	MeetingRoom();
	virtual ~MeetingRoom();

	virtual void Init() override;

	virtual void HandleClose() override;

	virtual void Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) override;

	virtual void Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_WAITING_LIST& pkt) override;
	virtual void Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_ACCEPT_WAIT& pkt) override;
	virtual void Handle_C_OFFICE_GET_HOST(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_HOST& pkt) override;
	virtual void Handle_C_OFFICE_BREAK(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_BREAK& pkt) override;
	virtual void Handle_C_OFFICE_KICK(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_KICK& pkt) override;
	virtual void Handle_C_OFFICE_GET_PERMISSION(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_PERMISSION& pkt) override;
	virtual void Handle_C_OFFICE_GET_PERMISSION_ALL(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_PERMISSION_ALL& pkt) override;
	virtual void Handle_C_OFFICE_SET_PERMISSION(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_SET_PERMISSION& pkt) override;
	virtual void Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_SET_ROOM_INFO& pkt) override;
	virtual void Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_ROOM_INFO& pkt) override;
	virtual void Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_VIDEO_STREAM& pkt) override;
	virtual void Handle_C_OFFICE_SHARE(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_SHARE& pkt) override;

	virtual void Leave(shared_ptr<ClientBase> client) override;

	virtual pair<bool, string> HandleEnter(const Protocol::C_ENTER& pkt) override;
	virtual void SetClientData(shared_ptr<ClientBase> client) override;
	virtual void OnEnterSuccess(shared_ptr<ClientBase> client) override;

	virtual shared_ptr<ClientBase> MakeClient(string clientId, int sessionId) override;

	void GetHost(shared_ptr<ClientBase> client);
	void Break(shared_ptr<ClientBase> client);
	void Kick(shared_ptr<ClientBase> client, string clientId);
	void GetPermission(shared_ptr<ClientBase> client, string clientId);
	void GetPermissionAll(shared_ptr<ClientBase> client);
	void SetPermission(shared_ptr<ClientBase> client, Protocol::C_OFFICE_SET_PERMISSION pkt);
	void GetRoomInfo(shared_ptr<ClientBase> client);
	void SetRoomInfo(shared_ptr<ClientBase> client, Protocol::C_OFFICE_SET_ROOM_INFO pkt);
	void HandleVideoStream(string url, float volume, float time, bool play, bool seek, int mediaplayerstate);
	void HandleShare(shared_ptr<ClientBase> client, bool isShared, int userId);

	void GetWaitingList(shared_ptr<ClientBase> client);
	void AcceptWait(shared_ptr<ClientBase> client, Protocol::C_OFFICE_ACCEPT_WAIT pkt);

	void Countdown();

	string creatorId;

	string roomCode;

	string roomName;
	string description;
	string spaceInfoId;
	int topicType;
	string thumbnail;
	string currentHostId;
	bool isPassword;
	string password;

	bool isAdvertising;
	bool isShutdown;

	bool isWaitingRoom;
	map<string, shared_ptr<MeetingClient>> waitingClients;

	int runningTime;
	int passedTime;

	time_t createdTime;
	string createdTimeString;
	string endTimeString;
};

