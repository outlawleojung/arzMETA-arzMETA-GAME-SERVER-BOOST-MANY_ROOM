#pragma once

#include "../Game/GameRoom.h"

#include <nlohmann/json.hpp>

class OfficeClient;

enum OfficeRoomUserType : int
{
	Host = 1,
	SubHost = 2,
	Guest = 3,
	Speaker = 4,
	Audience = 5,
	Observer = 6,
};

class OfficeRoom : public GameRoom
{
//public:
//	OfficeRoom();
//
//	virtual void Init() override;
//
//	virtual void HandleClose() override;
//
//	virtual void Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) override;
//	
//	virtual void Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<ClientBase>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt) override;
//	virtual void Handle_C_INTERACTION_SET_ITEM(shared_ptr<ClientBase>& session, Protocol::C_INTERACTION_SET_ITEM& pkt) override;
//
//	virtual void Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_WAITING_LIST& pkt) override;
//	virtual void Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_ACCEPT_WAIT& pkt) override;
//	virtual void Handle_C_OFFICE_GET_HOST(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_HOST& pkt) override;
//	virtual void Handle_C_OFFICE_BREAK(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_BREAK& pkt) override;
//	virtual void Handle_C_OFFICE_KICK(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_KICK& pkt) override;
//	virtual void Handle_C_OFFICE_GET_PERMISSION(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_PERMISSION& pkt) override;
//	virtual void Handle_C_OFFICE_SET_PERMISSION(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_SET_PERMISSION& pkt) override;
//	virtual void Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_SET_ROOM_INFO& pkt) override;
//	virtual void Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_ROOM_INFO& pkt) override;
//	virtual void Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_VIDEO_STREAM& pkt) override;
//	virtual void Handle_C_OFFICE_SHARE(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_SHARE& pkt) override;
//
//	//virtual void Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt) override;
//	virtual void Leave(shared_ptr<ClientBase> session) override;
//
//	virtual shared_ptr<ClientBase> MakeClient(string clientId, int sessionId) override;
//
//	void InstantiateObject(shared_ptr<ClientBase> session, Protocol::C_BASE_INSTANTIATE_OBJECT pkt);
//	void SetState(shared_ptr<ClientBase> session, Protocol::C_INTERACTION_SET_ITEM pkt);
//	void RemoveState(shared_ptr<ClientBase> session, Protocol::C_INTERACTION_REMOVE_ITEM pkt);
//
//	void GetHost(shared_ptr<ClientBase> session);
//	void Break(shared_ptr<ClientBase> session);
//	void Kick(shared_ptr<ClientBase> session, string clientId);
//	void GetPermission(shared_ptr<ClientBase> session, string clientId);
//	void SetPermission(shared_ptr<ClientBase> session, Protocol::C_OFFICE_SET_PERMISSION pkt);
//	void GetRoomInfo(shared_ptr<ClientBase> session);
//	void SetRoomInfo(shared_ptr<ClientBase> session, Protocol::C_OFFICE_SET_ROOM_INFO pkt);
//	void HandleVideoStream(string url, float volume, float time, bool play, bool seek, int mediaplayerstate);
//	void HandleShare(shared_ptr<ClientBase> session, bool isShared, int userId);
//
//	void GetWaitingList(shared_ptr<ClientBase> session);
//	void AcceptWait(shared_ptr<ClientBase> session, string clientId, bool isAccepted);
//
//	void Countdown();
//
//	string creatorId;
//
//	string roomCode;
//
//	string roomName;
//	string description;
//	string spaceInfoId;
//	int modeType;
//	int topicType;
//	string thumbnail;
//	string currentHostId;
//	string subHostMemberCode;
//	bool isPassword;
//	string password;
//	int currentPersonnel;
//	int observer;
//	int currentObserver;
//	bool isAdvertising;
//	bool isShutdown;
//
//	bool isWaitingRoom;
//	map<string, shared_ptr<OfficeClient>> waitingList;
//
//	int runningTime;
//	int passedTime;
//
//	time_t createdTime;
//	string createdTimeString;
//	string endTimeString;
//
//	bool AUTO_DESTROY = false;
//	bool DESTROY_WHEN_EMPTY = false;
};

