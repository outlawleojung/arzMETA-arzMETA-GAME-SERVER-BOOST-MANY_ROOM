#pragma once

#include "GameRoom.h"

#include <nlohmann/json.hpp>

enum OfficeRoomUserType : int
{
	Host = 1,
	SubHost = 2,
	Guest = 3,
	Speaker = 4,
	Audience = 5,
	Observer = 6,
};

class OfficeRoomSession
{
public:
	shared_ptr<GameSession> session;
	OfficeRoomUserType type;
	bool screenPermission;
	bool chatPermission;
	bool voicePermission;
	bool videoPermission;
};

class OfficeRoom : public GameRoom
{
public:
	OfficeRoom(vector<string> sceneIds);

	virtual void Init() override;

	void HandleClose();

	virtual void Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) override;
	virtual void Handle_C_REENTER(shared_ptr<GameSession>& session, Protocol::C_REENTER& pkt) override;
	virtual void Handle_C_LEAVE(shared_ptr<GameSession>& session, Protocol::C_LEAVE& pkt) override;
	virtual void Handle_C_SET_NICKNAME(shared_ptr<GameSession>& session, Protocol::C_SET_NICKNAME& pkt) override;
	
	virtual void Handle_C_BASE_SET_SCENE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_SCENE& pkt) override;
	virtual void Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt) override;

	virtual void Handle_C_INTERACTION_SET_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_SET_ITEM& pkt) override;

	virtual void Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_WAITING_LIST& pkt) override;
	virtual void Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_ACCEPT_WAIT& pkt) override;
	virtual void Handle_C_OFFICE_GET_CLIENT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_CLIENT& pkt) override;
	virtual void Handle_C_OFFICE_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_HOST& pkt) override;
	virtual void Handle_C_OFFICE_BREAK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_BREAK& pkt) override;
	virtual void Handle_C_OFFICE_KICK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_KICK& pkt) override;
	virtual void Handle_C_OFFICE_GET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_PERMISSION& pkt) override;
	virtual void Handle_C_OFFICE_SET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_PERMISSION& pkt) override;
	virtual void Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_ROOM_INFO& pkt) override;
	virtual void Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_ROOM_INFO& pkt) override;
	virtual void Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<GameSession>& session, Protocol::C_OFFICE_VIDEO_STREAM& pkt) override;

	virtual void Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt) override;
	virtual void ReEnter(shared_ptr<GameSession> session, string clientId) override;
	virtual void Leave(shared_ptr<GameSession> session) override;
	
	void SetNickname(shared_ptr<GameSession> session, string nickname);

	void SetScene(shared_ptr<GameSession> session, string sceneId);
	void InstantiateObject(shared_ptr<GameSession> session, Protocol::C_BASE_INSTANTIATE_OBJECT pkt);

	void SetState(shared_ptr<GameSession> session, Protocol::C_INTERACTION_SET_ITEM pkt);
	void RemoveState(shared_ptr<GameSession> session, Protocol::C_INTERACTION_REMOVE_ITEM pkt);

	void GetClient(shared_ptr<GameSession> session);
	void GetHost(shared_ptr<GameSession> session);
	void Break(shared_ptr<GameSession> session);
	void Kick(shared_ptr<GameSession> session, string clientId);
	void GetPermission(shared_ptr<GameSession> session, string clientId);
	void SetPermission(shared_ptr<GameSession> session, Protocol::C_OFFICE_SET_PERMISSION pkt);
	void GetRoomInfo(shared_ptr<GameSession> session);
	void SetRoomInfo(shared_ptr<GameSession> session, Protocol::C_OFFICE_SET_ROOM_INFO pkt);
	void HandleVideoStream(string url, float volume, float time, bool play, bool seek, int mediaplayerstate);

	void GetWaitingList(shared_ptr<GameSession> session);
	void AcceptWait(shared_ptr<GameSession> session, string clientId, bool isAccepted);

	void Countdown();

	virtual void Broadcast(shared_ptr<SendBuffer> sendBuffer) override;

	virtual nlohmann::json ToJson() override;

	string creatorId;

	string roomName;
	string description;
	string spaceInfoId;
	int modeType;
	int topicType;
	string thumbnail;
	string currentHostId;
	string subHostMemberCode;
	bool isPassword;
	string password;
	int personnel;
	int currentPersonnel;
	int observer;
	int currentObserver;
	bool isAdvertising;
	bool isShutdown;

	bool isWaitingRoom;
	map<string, pair<bool, shared_ptr<OfficeRoomSession>>> waitingList;

	int runningTime;
	int passedTime;

	time_t createdTime;
	string createdTimeString;
	string endTimeString;

	std::map<string, std::shared_ptr<OfficeRoomSession>> clients;

	bool AUTO_DESTROY = false;
	bool DESTROY_WHEN_EMPTY = false;
};

