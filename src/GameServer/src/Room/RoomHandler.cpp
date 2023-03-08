#include "../Util/pch.h"
#include "RoomBase.h"
void RoomBase::Handle_C_ENTER(shared_ptr<GameSession>& session, Protocol::C_ENTER& pkt)
{
    for (auto it = C_ENTER_Handlers.begin(); it != C_ENTER_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_REENTER(shared_ptr<GameSession>& session, Protocol::C_REENTER& pkt)
{
    for (auto it = C_REENTER_Handlers.begin(); it != C_REENTER_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_LEAVE(shared_ptr<GameSession>& session, Protocol::C_LEAVE& pkt)
{
    for (auto it = C_LEAVE_Handlers.begin(); it != C_LEAVE_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_SET_NICKNAME(shared_ptr<GameSession>& session, Protocol::C_SET_NICKNAME& pkt)
{
    for (auto it = C_SET_NICKNAME_Handlers.begin(); it != C_SET_NICKNAME_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_OFFICE_GET_WAITING_LIST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_WAITING_LIST& pkt)
{
    for (auto it = C_OFFICE_GET_WAITING_LIST_Handlers.begin(); it != C_OFFICE_GET_WAITING_LIST_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_OFFICE_ACCEPT_WAIT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_ACCEPT_WAIT& pkt)
{
    for (auto it = C_OFFICE_ACCEPT_WAIT_Handlers.begin(); it != C_OFFICE_ACCEPT_WAIT_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_OFFICE_GET_CLIENT(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_CLIENT& pkt)
{
    for (auto it = C_OFFICE_GET_CLIENT_Handlers.begin(); it != C_OFFICE_GET_CLIENT_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_OFFICE_GET_HOST(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_HOST& pkt)
{
    for (auto it = C_OFFICE_GET_HOST_Handlers.begin(); it != C_OFFICE_GET_HOST_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_OFFICE_BREAK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_BREAK& pkt)
{
    for (auto it = C_OFFICE_BREAK_Handlers.begin(); it != C_OFFICE_BREAK_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_OFFICE_KICK(shared_ptr<GameSession>& session, Protocol::C_OFFICE_KICK& pkt)
{
    for (auto it = C_OFFICE_KICK_Handlers.begin(); it != C_OFFICE_KICK_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_OFFICE_GET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_PERMISSION& pkt)
{
    for (auto it = C_OFFICE_GET_PERMISSION_Handlers.begin(); it != C_OFFICE_GET_PERMISSION_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_OFFICE_SET_PERMISSION(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_PERMISSION& pkt)
{
    for (auto it = C_OFFICE_SET_PERMISSION_Handlers.begin(); it != C_OFFICE_SET_PERMISSION_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_OFFICE_SET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_SET_ROOM_INFO& pkt)
{
    for (auto it = C_OFFICE_SET_ROOM_INFO_Handlers.begin(); it != C_OFFICE_SET_ROOM_INFO_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_OFFICE_GET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::C_OFFICE_GET_ROOM_INFO& pkt)
{
    for (auto it = C_OFFICE_GET_ROOM_INFO_Handlers.begin(); it != C_OFFICE_GET_ROOM_INFO_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_OFFICE_VIDEO_STREAM(shared_ptr<GameSession>& session, Protocol::C_OFFICE_VIDEO_STREAM& pkt)
{
    for (auto it = C_OFFICE_VIDEO_STREAM_Handlers.begin(); it != C_OFFICE_VIDEO_STREAM_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_MYROOM_GET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_GET_ROOMINFO& pkt)
{
    for (auto it = C_MYROOM_GET_ROOMINFO_Handlers.begin(); it != C_MYROOM_GET_ROOMINFO_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_MYROOM_SET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::C_MYROOM_SET_ROOMINFO& pkt)
{
    for (auto it = C_MYROOM_SET_ROOMINFO_Handlers.begin(); it != C_MYROOM_SET_ROOMINFO_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_MYROOM_OTHER_ROOM_LIST(shared_ptr<GameSession>& session, Protocol::C_MYROOM_OTHER_ROOM_LIST& pkt)
{
    for (auto it = C_MYROOM_OTHER_ROOM_LIST_Handlers.begin(); it != C_MYROOM_OTHER_ROOM_LIST_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_BASE_SET_SCENE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_SCENE& pkt)
{
    for (auto it = C_BASE_SET_SCENE_Handlers.begin(); it != C_BASE_SET_SCENE_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_BASE_INSTANTIATE_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_INSTANTIATE_OBJECT& pkt)
{
    for (auto it = C_BASE_INSTANTIATE_OBJECT_Handlers.begin(); it != C_BASE_INSTANTIATE_OBJECT_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_BASE_GET_OBJECT(shared_ptr<GameSession>& session, Protocol::C_BASE_GET_OBJECT& pkt)
{
    for (auto it = C_BASE_GET_OBJECT_Handlers.begin(); it != C_BASE_GET_OBJECT_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_BASE_SET_OBJECT_DATA(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_OBJECT_DATA& pkt)
{
    for (auto it = C_BASE_SET_OBJECT_DATA_Handlers.begin(); it != C_BASE_SET_OBJECT_DATA_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_BASE_SET_TRANSFORM(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_TRANSFORM& pkt)
{
    for (auto it = C_BASE_SET_TRANSFORM_Handlers.begin(); it != C_BASE_SET_TRANSFORM_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_BASE_SET_ANIMATION(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION& pkt)
{
    for (auto it = C_BASE_SET_ANIMATION_Handlers.begin(); it != C_BASE_SET_ANIMATION_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_BASE_SET_ANIMATION_ONCE(shared_ptr<GameSession>& session, Protocol::C_BASE_SET_ANIMATION_ONCE& pkt)
{
    for (auto it = C_BASE_SET_ANIMATION_ONCE_Handlers.begin(); it != C_BASE_SET_ANIMATION_ONCE_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_INTERACTION_GET_ITEMS(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_GET_ITEMS& pkt)
{
    for (auto it = C_INTERACTION_GET_ITEMS_Handlers.begin(); it != C_INTERACTION_GET_ITEMS_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_INTERACTION_SET_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_SET_ITEM& pkt)
{
    for (auto it = C_INTERACTION_SET_ITEM_Handlers.begin(); it != C_INTERACTION_SET_ITEM_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
void RoomBase::Handle_C_INTERACTION_REMOVE_ITEM(shared_ptr<GameSession>& session, Protocol::C_INTERACTION_REMOVE_ITEM& pkt)
{
    for (auto it = C_INTERACTION_REMOVE_ITEM_Handlers.begin(); it != C_INTERACTION_REMOVE_ITEM_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}