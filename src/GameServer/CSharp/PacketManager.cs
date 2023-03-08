using Google.Protobuf;
using Protocol;
using System;
using System.Collections.Generic;
using FrameWork.Network;

public class PacketManager
{
	// #region Singleton

	// private static PacketManager instance = new PacketManager();
	// public static PacketManager Instance { get { return instance; } }
	// #endregion

	public PacketManager()
	{
		Register();
	}

	public void Clear()
	{
		_onRecv.Clear();
		_handler.Clear();
		CustomHandler = null;
	}

	readonly Dictionary<ushort, Action<PacketSession, ArraySegment<byte>, ushort>> _onRecv = new Dictionary<ushort, Action<PacketSession, ArraySegment<byte>, ushort>>();
	readonly Dictionary<ushort, Dictionary<object, Action<PacketSession, IMessage>>> _handler = new Dictionary<ushort, Dictionary<object, Action<PacketSession, IMessage>>>();
	public Action<PacketSession, IMessage, ushort> CustomHandler { get; set; }

	public enum MsgId : ushort
    {
	    PKT_C_ENTER = 0,
	    PKT_S_ENTER = 1,
	    PKT_C_REENTER = 2,
	    PKT_S_REENTER = 3,
	    PKT_C_LEAVE = 4,
	    PKT_C_SET_NICKNAME = 5,
	    PKT_S_SET_NICKNAME = 6,
	    PKT_S_SET_NICKNAME_NOTICE = 7,
	    PKT_S_DISCONNECT = 8,
	    PKT_C_OFFICE_GET_WAITING_LIST = 100,
	    PKT_S_OFFICE_ADD_WAITING_CLIENT = 101,
	    PKT_S_OFFICE_REMOVE_WAITING_CLIENT = 102,
	    PKT_C_OFFICE_ACCEPT_WAIT = 103,
	    PKT_S_OFFICE_ACCEPT_WAIT = 104,
	    PKT_S_OFFICE_ACCEPT_WAIT_NOTICE = 105,
	    PKT_C_OFFICE_GET_CLIENT = 106,
	    PKT_S_OFFICE_ADD_CLIENT = 107,
	    PKT_S_OFFICE_REMOVE_CLIENT = 108,
	    PKT_C_OFFICE_GET_HOST = 109,
	    PKT_S_OFFICE_GET_HOST = 110,
	    PKT_C_OFFICE_BREAK = 111,
	    PKT_S_OFFICE_BREAK = 112,
	    PKT_C_OFFICE_KICK = 113,
	    PKT_S_OFFICE_KICK = 114,
	    PKT_C_OFFICE_GET_PERMISSION = 115,
	    PKT_S_OFFICE_GET_PERMISSION = 116,
	    PKT_C_OFFICE_SET_PERMISSION = 117,
	    PKT_S_OFFICE_SET_PERMISSION = 118,
	    PKT_S_OFFICE_SET_PERMISSION_NOTICE = 119,
	    PKT_C_OFFICE_SET_ROOM_INFO = 120,
	    PKT_S_OFFICE_SET_ROOM_INFO = 121,
	    PKT_C_OFFICE_GET_ROOM_INFO = 122,
	    PKT_S_OFFICE_GET_ROOM_INFO = 123,
	    PKT_C_OFFICE_VIDEO_STREAM = 124,
	    PKT_S_OFFICE_VIDEO_STREAM = 125,
	    PKT_C_MYROOM_GET_ROOMINFO = 200,
	    PKT_S_MYROOM_GET_ROOMINFO = 201,
	    PKT_C_MYROOM_SET_ROOMINFO = 202,
	    PKT_S_MYROOM_SET_ROOMINFO = 203,
	    PKT_C_MYROOM_OTHER_ROOM_LIST = 204,
	    PKT_S_MYROOM_OTHER_ROOM_LIST = 205,
	    PKT_C_BASE_SET_SCENE = 10100,
	    PKT_S_BASE_SET_SCENE = 10101,
	    PKT_C_BASE_INSTANTIATE_OBJECT = 10102,
	    PKT_S_BASE_INSTANTIATE_OBJECT = 10103,
	    PKT_C_BASE_GET_OBJECT = 10104,
	    PKT_S_BASE_ADD_OBJECT = 10105,
	    PKT_S_BASE_REMOVE_OBJECT = 10106,
	    PKT_C_BASE_SET_OBJECT_DATA = 10107,
	    PKT_S_BASE_SET_OBJECT_DATA = 10108,
	    PKT_S_BASE_SET_OBJECT_DATA_NOTICE = 10109,
	    PKT_C_BASE_SET_TRANSFORM = 10110,
	    PKT_S_BASE_SET_TRANSFORM = 10111,
	    PKT_C_BASE_SET_ANIMATION = 10112,
	    PKT_S_BASE_SET_ANIMATION = 10113,
	    PKT_C_BASE_SET_ANIMATION_ONCE = 10114,
	    PKT_S_BASE_SET_ANIMATION_ONCE = 10115,
	    PKT_C_INTERACTION_GET_ITEMS = 10200,
	    PKT_S_INTERACTION_GET_ITEMS = 10201,
	    PKT_C_INTERACTION_SET_ITEM = 10202,
	    PKT_S_INTERACTION_SET_ITEM = 10203,
	    PKT_S_INTERACTION_SET_ITEM_NOTICE = 10204,
	    PKT_C_INTERACTION_REMOVE_ITEM = 10205,
	    PKT_S_INTERACTION_REMOVE_ITEM = 10206,
	    PKT_S_INTERACTION_REMOVE_ITEM_NOTICE = 10207,
	}

	public void Register()
	{
        _onRecv.Add((ushort)MsgId.PKT_S_ENTER, MakePacket<S_ENTER>);
		_handler.Add((ushort)MsgId.PKT_S_ENTER, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_REENTER, MakePacket<S_REENTER>);
		_handler.Add((ushort)MsgId.PKT_S_REENTER, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_SET_NICKNAME, MakePacket<S_SET_NICKNAME>);
		_handler.Add((ushort)MsgId.PKT_S_SET_NICKNAME, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_SET_NICKNAME_NOTICE, MakePacket<S_SET_NICKNAME_NOTICE>);
		_handler.Add((ushort)MsgId.PKT_S_SET_NICKNAME_NOTICE, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_DISCONNECT, MakePacket<S_DISCONNECT>);
		_handler.Add((ushort)MsgId.PKT_S_DISCONNECT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_ADD_WAITING_CLIENT, MakePacket<S_OFFICE_ADD_WAITING_CLIENT>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_ADD_WAITING_CLIENT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_REMOVE_WAITING_CLIENT, MakePacket<S_OFFICE_REMOVE_WAITING_CLIENT>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_REMOVE_WAITING_CLIENT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_ACCEPT_WAIT, MakePacket<S_OFFICE_ACCEPT_WAIT>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_ACCEPT_WAIT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_ACCEPT_WAIT_NOTICE, MakePacket<S_OFFICE_ACCEPT_WAIT_NOTICE>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_ACCEPT_WAIT_NOTICE, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_ADD_CLIENT, MakePacket<S_OFFICE_ADD_CLIENT>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_ADD_CLIENT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_REMOVE_CLIENT, MakePacket<S_OFFICE_REMOVE_CLIENT>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_REMOVE_CLIENT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_GET_HOST, MakePacket<S_OFFICE_GET_HOST>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_GET_HOST, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_BREAK, MakePacket<S_OFFICE_BREAK>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_BREAK, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_KICK, MakePacket<S_OFFICE_KICK>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_KICK, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_GET_PERMISSION, MakePacket<S_OFFICE_GET_PERMISSION>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_GET_PERMISSION, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_SET_PERMISSION, MakePacket<S_OFFICE_SET_PERMISSION>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_SET_PERMISSION, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_SET_PERMISSION_NOTICE, MakePacket<S_OFFICE_SET_PERMISSION_NOTICE>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_SET_PERMISSION_NOTICE, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_SET_ROOM_INFO, MakePacket<S_OFFICE_SET_ROOM_INFO>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_SET_ROOM_INFO, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_GET_ROOM_INFO, MakePacket<S_OFFICE_GET_ROOM_INFO>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_GET_ROOM_INFO, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_VIDEO_STREAM, MakePacket<S_OFFICE_VIDEO_STREAM>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_VIDEO_STREAM, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MYROOM_GET_ROOMINFO, MakePacket<S_MYROOM_GET_ROOMINFO>);
		_handler.Add((ushort)MsgId.PKT_S_MYROOM_GET_ROOMINFO, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MYROOM_SET_ROOMINFO, MakePacket<S_MYROOM_SET_ROOMINFO>);
		_handler.Add((ushort)MsgId.PKT_S_MYROOM_SET_ROOMINFO, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MYROOM_OTHER_ROOM_LIST, MakePacket<S_MYROOM_OTHER_ROOM_LIST>);
		_handler.Add((ushort)MsgId.PKT_S_MYROOM_OTHER_ROOM_LIST, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_BASE_SET_SCENE, MakePacket<S_BASE_SET_SCENE>);
		_handler.Add((ushort)MsgId.PKT_S_BASE_SET_SCENE, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_BASE_INSTANTIATE_OBJECT, MakePacket<S_BASE_INSTANTIATE_OBJECT>);
		_handler.Add((ushort)MsgId.PKT_S_BASE_INSTANTIATE_OBJECT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_BASE_ADD_OBJECT, MakePacket<S_BASE_ADD_OBJECT>);
		_handler.Add((ushort)MsgId.PKT_S_BASE_ADD_OBJECT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_BASE_REMOVE_OBJECT, MakePacket<S_BASE_REMOVE_OBJECT>);
		_handler.Add((ushort)MsgId.PKT_S_BASE_REMOVE_OBJECT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_BASE_SET_OBJECT_DATA, MakePacket<S_BASE_SET_OBJECT_DATA>);
		_handler.Add((ushort)MsgId.PKT_S_BASE_SET_OBJECT_DATA, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_BASE_SET_OBJECT_DATA_NOTICE, MakePacket<S_BASE_SET_OBJECT_DATA_NOTICE>);
		_handler.Add((ushort)MsgId.PKT_S_BASE_SET_OBJECT_DATA_NOTICE, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_BASE_SET_TRANSFORM, MakePacket<S_BASE_SET_TRANSFORM>);
		_handler.Add((ushort)MsgId.PKT_S_BASE_SET_TRANSFORM, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_BASE_SET_ANIMATION, MakePacket<S_BASE_SET_ANIMATION>);
		_handler.Add((ushort)MsgId.PKT_S_BASE_SET_ANIMATION, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_BASE_SET_ANIMATION_ONCE, MakePacket<S_BASE_SET_ANIMATION_ONCE>);
		_handler.Add((ushort)MsgId.PKT_S_BASE_SET_ANIMATION_ONCE, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_INTERACTION_GET_ITEMS, MakePacket<S_INTERACTION_GET_ITEMS>);
		_handler.Add((ushort)MsgId.PKT_S_INTERACTION_GET_ITEMS, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_INTERACTION_SET_ITEM, MakePacket<S_INTERACTION_SET_ITEM>);
		_handler.Add((ushort)MsgId.PKT_S_INTERACTION_SET_ITEM, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_INTERACTION_SET_ITEM_NOTICE, MakePacket<S_INTERACTION_SET_ITEM_NOTICE>);
		_handler.Add((ushort)MsgId.PKT_S_INTERACTION_SET_ITEM_NOTICE, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_INTERACTION_REMOVE_ITEM, MakePacket<S_INTERACTION_REMOVE_ITEM>);
		_handler.Add((ushort)MsgId.PKT_S_INTERACTION_REMOVE_ITEM, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_INTERACTION_REMOVE_ITEM_NOTICE, MakePacket<S_INTERACTION_REMOVE_ITEM_NOTICE>);
		_handler.Add((ushort)MsgId.PKT_S_INTERACTION_REMOVE_ITEM_NOTICE, new Dictionary<object, Action<PacketSession, IMessage>>());
	}

	public void AddHandler(MsgId msgId, object obj, Action<PacketSession, IMessage> handler)
	{
		Dictionary<object, Action<PacketSession, IMessage>> list;
		if (_handler.TryGetValue((ushort) msgId, out list))
		{
			list.Add(obj, handler);
		}
	}

	public void RemoveHandler(MsgId msgId, object obj)
	{
		Dictionary<object, Action<PacketSession, IMessage>> list;
		if (_handler.TryGetValue((ushort) msgId, out list))
		{
			list.Remove(obj);
		}
	}
	
	public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
	{
		ushort count = 0;

		ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
		count += 2;
		ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
		count += 2;

		Action<PacketSession, ArraySegment<byte>, ushort> action = null;
		if (_onRecv.TryGetValue(id, out action))
			action.Invoke(session, buffer, id);
	}

	void MakePacket<T>(PacketSession session, ArraySegment<byte> buffer, ushort id) where T : IMessage, new()
	{
		T pkt = new T();
		pkt.MergeFrom(buffer.Array, buffer.Offset + 4, buffer.Count - 4);

		CustomHandler?.Invoke(session, pkt, id);
	}

	public Dictionary<object, Action<PacketSession, IMessage>> GetPacketHandler(ushort id)
	{
		Dictionary<object, Action<PacketSession, IMessage>> action = null;
		if (_handler.TryGetValue(id, out action))
			return action;
		return null;
	}
}