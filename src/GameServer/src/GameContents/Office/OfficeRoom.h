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
public:
	OfficeRoom(vector<string> sceneIds);

	virtual void Init() override;

	virtual void HandleClose() override;

	virtual void Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt) override;

	virtual void Handle_C_SET_NICKNAME(shared_ptr<ClientBase>& session, Protocol::C_SET_NICKNAME& pkt) override;
	
	virtual void Handle_C_BASE_SET_SCENE(shared_ptr<ClientBase>& session, Protocol::C_BASE_SET_SCENE& pkt) override;
	virtual void Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<ClientBase>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt) override;

	virtual void Handle_C_INTERACTION_SET_ITEM(shared_ptr<ClientBase>& session, Protocol::C_INTERACTION_SET_ITEM& pkt) override;

	virtual void Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_WAITING_LIST& pkt) override;
	virtual void Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_ACCEPT_WAIT& pkt) override;
	virtual void Handle_C_OFFICE_GET_CLIENT(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_CLIENT& pkt) override;
	virtual void Handle_C_OFFICE_GET_HOST(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_HOST& pkt) override;
	virtual void Handle_C_OFFICE_BREAK(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_BREAK& pkt) override;
	virtual void Handle_C_OFFICE_KICK(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_KICK& pkt) override;
	virtual void Handle_C_OFFICE_GET_PERMISSION(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_PERMISSION& pkt) override;
	virtual void Handle_C_OFFICE_SET_PERMISSION(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_SET_PERMISSION& pkt) override;
	virtual void Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_SET_ROOM_INFO& pkt) override;
	virtual void Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_GET_ROOM_INFO& pkt) override;
	virtual void Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<ClientBase>& session, Protocol::C_OFFICE_VIDEO_STREAM& pkt) override;

	virtual void Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt) override;
	virtual void Leave(shared_ptr<ClientBase> session) override;
	
	void SetNickname(shared_ptr<ClientBase> session, string nickname);

	void SetScene(shared_ptr<ClientBase> session, string sceneId);
	void InstantiateObject(shared_ptr<ClientBase> session, Protocol::C_BASE_INSTANTIATE_OBJECT pkt);

	void SetState(shared_ptr<ClientBase> session, Protocol::C_INTERACTION_SET_ITEM pkt);
	void RemoveState(shared_ptr<ClientBase> session, Protocol::C_INTERACTION_REMOVE_ITEM pkt);

	void GetClient(shared_ptr<ClientBase> session);
	void GetHost(shared_ptr<ClientBase> session);
	void Break(shared_ptr<ClientBase> session);
	void Kick(shared_ptr<ClientBase> session, string clientId);
	void GetPermission(shared_ptr<ClientBase> session, string clientId);
	void SetPermission(shared_ptr<ClientBase> session, Protocol::C_OFFICE_SET_PERMISSION pkt);
	void GetRoomInfo(shared_ptr<ClientBase> session);
	void SetRoomInfo(shared_ptr<ClientBase> session, Protocol::C_OFFICE_SET_ROOM_INFO pkt);
	void HandleVideoStream(string url, float volume, float time, bool play, bool seek, int mediaplayerstate);

	void GetWaitingList(shared_ptr<ClientBase> session);
	void AcceptWait(shared_ptr<ClientBase> session, string clientId, bool isAccepted);

	void Countdown();

	virtual nlohmann::json ToJson() override;

	string creatorId;

	string roomCode;

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
	//int personnel
	int currentPersonnel;
	int observer;
	int currentObserver;
	bool isAdvertising;
	bool isShutdown;

	bool isWaitingRoom;
	map<string, pair<bool, shared_ptr<OfficeClient>>> waitingList;

	int runningTime;
	int passedTime;

	time_t createdTime;
	string createdTimeString;
	string endTimeString;

	//std::map<string, std::shared_ptr<OfficeClient>> clients;

	bool AUTO_DESTROY = false;
	bool DESTROY_WHEN_EMPTY = false;
};

