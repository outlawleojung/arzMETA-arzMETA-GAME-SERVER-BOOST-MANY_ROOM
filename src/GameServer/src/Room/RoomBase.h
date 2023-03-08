#pragma once

#include "../Util/pch.h"
#include "../Util/Protocols.h"

#include <nlohmann/json.hpp>

class GameSession;

enum class RoomState
{
	Idle,
	Running,
	Closing,
	Closed
};

class RoomBase : public JobQueue
{
public:
	RoomBase();
	virtual ~RoomBase();

	virtual void Init();
	virtual void Close();
	virtual void HandleClose() = 0;
	virtual void Clear() = 0;
	
	virtual void Enter(shared_ptr<GameSession> session, Protocol::C_ENTER pkt) = 0;
	virtual void ReEnter(shared_ptr<GameSession> session, string clientId) = 0;	
	virtual void Leave(shared_ptr<GameSession> session) = 0;

	virtual void Broadcast(shared_ptr<SendBuffer> sendBuffer) = 0;

	virtual nlohmann::json ToJson() = 0;
	
	string roomId;
	std::atomic<RoomState> state;
	std::function<void(std::shared_ptr<GameSession>)> onDisconnected;

protected:
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_ENTER&)>> C_ENTER_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_REENTER&)>> C_REENTER_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_LEAVE&)>> C_LEAVE_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_SET_NICKNAME&)>> C_SET_NICKNAME_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_OFFICE_GET_WAITING_LIST&)>> C_OFFICE_GET_WAITING_LIST_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_OFFICE_ACCEPT_WAIT&)>> C_OFFICE_ACCEPT_WAIT_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_OFFICE_GET_CLIENT&)>> C_OFFICE_GET_CLIENT_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_OFFICE_GET_HOST&)>> C_OFFICE_GET_HOST_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_OFFICE_BREAK&)>> C_OFFICE_BREAK_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_OFFICE_KICK&)>> C_OFFICE_KICK_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_OFFICE_GET_PERMISSION&)>> C_OFFICE_GET_PERMISSION_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_OFFICE_SET_PERMISSION&)>> C_OFFICE_SET_PERMISSION_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_OFFICE_SET_ROOM_INFO&)>> C_OFFICE_SET_ROOM_INFO_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_OFFICE_GET_ROOM_INFO&)>> C_OFFICE_GET_ROOM_INFO_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_OFFICE_VIDEO_STREAM&)>> C_OFFICE_VIDEO_STREAM_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_MYROOM_GET_ROOMINFO&)>> C_MYROOM_GET_ROOMINFO_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_MYROOM_SET_ROOMINFO&)>> C_MYROOM_SET_ROOMINFO_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_MYROOM_OTHER_ROOM_LIST&)>> C_MYROOM_OTHER_ROOM_LIST_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_BASE_SET_SCENE&)>> C_BASE_SET_SCENE_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_BASE_INSTANTIATE_OBJECT&)>> C_BASE_INSTANTIATE_OBJECT_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_BASE_GET_OBJECT&)>> C_BASE_GET_OBJECT_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_BASE_SET_OBJECT_DATA&)>> C_BASE_SET_OBJECT_DATA_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_BASE_SET_TRANSFORM&)>> C_BASE_SET_TRANSFORM_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_BASE_SET_ANIMATION&)>> C_BASE_SET_ANIMATION_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_BASE_SET_ANIMATION_ONCE&)>> C_BASE_SET_ANIMATION_ONCE_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_INTERACTION_GET_ITEMS&)>> C_INTERACTION_GET_ITEMS_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_INTERACTION_SET_ITEM&)>> C_INTERACTION_SET_ITEM_Handlers;
	std::vector<std::function<void(shared_ptr<GameSession>&, Protocol::C_INTERACTION_REMOVE_ITEM&)>> C_INTERACTION_REMOVE_ITEM_Handlers;

public:
	virtual void Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt);
	virtual void Handle_C_REENTER(shared_ptr<GameSession>& session, Protocol::C_REENTER& pkt);
	virtual void Handle_C_LEAVE(shared_ptr<GameSession>& session, Protocol::C_LEAVE& pkt);
	virtual void Handle_C_SET_NICKNAME(shared_ptr<GameSession>& session, Protocol::C_SET_NICKNAME& pkt);
	virtual void Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_WAITING_LIST& pkt);
	virtual void Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_ACCEPT_WAIT& pkt);
	virtual void Handle_C_OFFICE_GET_CLIENT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_CLIENT& pkt);
	virtual void Handle_C_OFFICE_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_HOST& pkt);
	virtual void Handle_C_OFFICE_BREAK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_BREAK& pkt);
	virtual void Handle_C_OFFICE_KICK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_KICK& pkt);
	virtual void Handle_C_OFFICE_GET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_PERMISSION& pkt);
	virtual void Handle_C_OFFICE_SET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_PERMISSION& pkt);
	virtual void Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_ROOM_INFO& pkt);
	virtual void Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_ROOM_INFO& pkt);
	virtual void Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<GameSession>& session, Protocol::C_OFFICE_VIDEO_STREAM& pkt);
	virtual void Handle_C_MYROOM_GET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_GET_ROOMINFO& pkt);
	virtual void Handle_C_MYROOM_SET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_SET_ROOMINFO& pkt);
	virtual void Handle_C_MYROOM_OTHER_ROOM_LIST(shared_ptr<GameSession>& session, Protocol::C_MYROOM_OTHER_ROOM_LIST& pkt);
	virtual void Handle_C_BASE_SET_SCENE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_SCENE& pkt);
	virtual void Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt);
	virtual void Handle_C_BASE_GET_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_GET_OBJECT& pkt);
	virtual void Handle_C_BASE_SET_OBJECT_DATA(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_OBJECT_DATA& pkt);
	virtual void Handle_C_BASE_SET_TRANSFORM(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_TRANSFORM& pkt);
	virtual void Handle_C_BASE_SET_ANIMATION(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION& pkt);
	virtual void Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt);
	virtual void Handle_C_INTERACTION_GET_ITEMS(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_GET_ITEMS& pkt);
	virtual void Handle_C_INTERACTION_SET_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_SET_ITEM& pkt);
	virtual void Handle_C_INTERACTION_REMOVE_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_REMOVE_ITEM& pkt);
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_ENTER& pkt))
	{
		C_ENTER_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_REENTER& pkt))
	{
		C_REENTER_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_REENTER& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_LEAVE& pkt))
	{
		C_LEAVE_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_LEAVE& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_SET_NICKNAME& pkt))
	{
		C_SET_NICKNAME_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_SET_NICKNAME& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_OFFICE_GET_WAITING_LIST& pkt))
	{
		C_OFFICE_GET_WAITING_LIST_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_WAITING_LIST& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_OFFICE_ACCEPT_WAIT& pkt))
	{
		C_OFFICE_ACCEPT_WAIT_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_OFFICE_ACCEPT_WAIT& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_OFFICE_GET_CLIENT& pkt))
	{
		C_OFFICE_GET_CLIENT_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_CLIENT& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_OFFICE_GET_HOST& pkt))
	{
		C_OFFICE_GET_HOST_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_HOST& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_OFFICE_BREAK& pkt))
	{
		C_OFFICE_BREAK_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_OFFICE_BREAK& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_OFFICE_KICK& pkt))
	{
		C_OFFICE_KICK_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_OFFICE_KICK& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_OFFICE_GET_PERMISSION& pkt))
	{
		C_OFFICE_GET_PERMISSION_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_PERMISSION& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_OFFICE_SET_PERMISSION& pkt))
	{
		C_OFFICE_SET_PERMISSION_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_PERMISSION& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_OFFICE_SET_ROOM_INFO& pkt))
	{
		C_OFFICE_SET_ROOM_INFO_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_ROOM_INFO& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_OFFICE_GET_ROOM_INFO& pkt))
	{
		C_OFFICE_GET_ROOM_INFO_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_ROOM_INFO& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_OFFICE_VIDEO_STREAM& pkt))
	{
		C_OFFICE_VIDEO_STREAM_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_OFFICE_VIDEO_STREAM& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_MYROOM_GET_ROOMINFO& pkt))
	{
		C_MYROOM_GET_ROOMINFO_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_MYROOM_GET_ROOMINFO& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_MYROOM_SET_ROOMINFO& pkt))
	{
		C_MYROOM_SET_ROOMINFO_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_MYROOM_SET_ROOMINFO& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_MYROOM_OTHER_ROOM_LIST& pkt))
	{
		C_MYROOM_OTHER_ROOM_LIST_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_MYROOM_OTHER_ROOM_LIST& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_BASE_SET_SCENE& pkt))
	{
		C_BASE_SET_SCENE_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_BASE_SET_SCENE& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt))
	{
		C_BASE_INSTANTIATE_OBJECT_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_BASE_GET_OBJECT& pkt))
	{
		C_BASE_GET_OBJECT_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_BASE_GET_OBJECT& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_BASE_SET_OBJECT_DATA& pkt))
	{
		C_BASE_SET_OBJECT_DATA_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_BASE_SET_OBJECT_DATA& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_BASE_SET_TRANSFORM& pkt))
	{
		C_BASE_SET_TRANSFORM_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_BASE_SET_TRANSFORM& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_BASE_SET_ANIMATION& pkt))
	{
		C_BASE_SET_ANIMATION_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt))
	{
		C_BASE_SET_ANIMATION_ONCE_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_INTERACTION_GET_ITEMS& pkt))
	{
		C_INTERACTION_GET_ITEMS_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_INTERACTION_GET_ITEMS& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_INTERACTION_SET_ITEM& pkt))
	{
		C_INTERACTION_SET_ITEM_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_INTERACTION_SET_ITEM& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
	template<typename T>
	void AddHandler(shared_ptr<T> owner, void(T::* handler)(shared_ptr<GameSession>&, Protocol::C_INTERACTION_REMOVE_ITEM& pkt))
	{
		C_INTERACTION_REMOVE_ITEM_Handlers.push_back([owner, handler](std::shared_ptr<GameSession>& session, Protocol::C_INTERACTION_REMOVE_ITEM& pkt)
			{
				(owner.get()->*handler)(session, pkt);
			}
    	);
	}
};