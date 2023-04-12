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
	    PKT_C_GET_CLIENT = 5,
	    PKT_S_ADD_CLIENT = 6,
	    PKT_S_REMOVE_CLIENT = 7,
	    PKT_C_SET_NICKNAME = 8,
	    PKT_S_SET_NICKNAME = 9,
	    PKT_S_SET_NICKNAME_NOTICE = 10,
	    PKT_S_DISCONNECT = 11,
	    PKT_C_BASE_SET_SCENE = 100,
	    PKT_S_BASE_SET_SCENE = 101,
	    PKT_C_BASE_INSTANTIATE_OBJECT = 102,
	    PKT_S_BASE_INSTANTIATE_OBJECT = 103,
	    PKT_C_BASE_GET_OBJECT = 104,
	    PKT_S_BASE_ADD_OBJECT = 105,
	    PKT_S_BASE_REMOVE_OBJECT = 106,
	    PKT_C_BASE_SET_OBJECT_DATA = 107,
	    PKT_S_BASE_SET_OBJECT_DATA = 108,
	    PKT_S_BASE_SET_OBJECT_DATA_NOTICE = 109,
	    PKT_C_BASE_SET_TRANSFORM = 110,
	    PKT_S_BASE_SET_TRANSFORM = 111,
	    PKT_C_BASE_SET_ANIMATION = 112,
	    PKT_S_BASE_SET_ANIMATION = 113,
	    PKT_C_BASE_SET_ANIMATION_ONCE = 114,
	    PKT_S_BASE_SET_ANIMATION_ONCE = 115,
	    PKT_C_INTERACTION_GET_ITEMS = 200,
	    PKT_S_INTERACTION_GET_ITEMS = 201,
	    PKT_C_INTERACTION_SET_ITEM = 202,
	    PKT_S_INTERACTION_SET_ITEM = 203,
	    PKT_S_INTERACTION_SET_ITEM_NOTICE = 204,
	    PKT_C_INTERACTION_REMOVE_ITEM = 205,
	    PKT_S_INTERACTION_REMOVE_ITEM = 206,
	    PKT_S_INTERACTION_REMOVE_ITEM_NOTICE = 207,
	    PKT_C_MYROOM_GET_ROOMINFO = 300,
	    PKT_S_MYROOM_GET_ROOMINFO = 301,
	    PKT_C_MYROOM_SET_ROOMINFO = 302,
	    PKT_S_MYROOM_SET_ROOMINFO = 303,
	    PKT_C_MYROOM_OTHER_ROOM_LIST = 304,
	    PKT_S_MYROOM_OTHER_ROOM_LIST = 305,
	    PKT_C_OFFICE_GET_WAITING_LIST = 400,
	    PKT_S_OFFICE_ADD_WAITING_CLIENT = 401,
	    PKT_S_OFFICE_REMOVE_WAITING_CLIENT = 402,
	    PKT_C_OFFICE_ACCEPT_WAIT = 403,
	    PKT_S_OFFICE_ACCEPT_WAIT = 404,
	    PKT_S_OFFICE_ACCEPT_WAIT_NOTICE = 405,
	    PKT_C_OFFICE_GET_HOST = 406,
	    PKT_S_OFFICE_GET_HOST = 407,
	    PKT_C_OFFICE_BREAK = 408,
	    PKT_S_OFFICE_BREAK = 409,
	    PKT_C_OFFICE_KICK = 410,
	    PKT_S_OFFICE_KICK = 411,
	    PKT_C_OFFICE_GET_PERMISSION = 412,
	    PKT_S_OFFICE_GET_PERMISSION = 413,
	    PKT_C_OFFICE_SET_PERMISSION = 414,
	    PKT_S_OFFICE_SET_PERMISSION = 415,
	    PKT_S_OFFICE_SET_PERMISSION_NOTICE = 416,
	    PKT_C_OFFICE_SET_ROOM_INFO = 417,
	    PKT_S_OFFICE_SET_ROOM_INFO = 418,
	    PKT_C_OFFICE_GET_ROOM_INFO = 419,
	    PKT_S_OFFICE_GET_ROOM_INFO = 420,
	    PKT_C_OFFICE_VIDEO_STREAM = 421,
	    PKT_S_OFFICE_VIDEO_STREAM = 422,
	    PKT_C_OFFICE_SHARE = 423,
	    PKT_S_OFFICE_SHARE = 424,
	    PKT_C_MATCHING_START = 500,
	    PKT_S_MATCHING_START = 501,
	    PKT_S_MATCHING_FINISH = 502,
	    PKT_C_MATCHING_GET_HOST = 503,
	    PKT_S_MATCHING_HOST = 504,
	    PKT_S_MATCHING_ROUND_START = 505,
	    PKT_S_MATCHING_ROUND_FINISH = 506,
	    PKT_S_MATCHING_TILES = 507,
	    PKT_S_MATCHING_HINT = 508,
	    PKT_S_MATCHING_PROBLEM = 509,
	    PKT_S_MATCHING_DESTROY = 510,
	    PKT_S_MATCHING_QUIZ_DISAPPEAR = 511,
	    PKT_C_MATCHING_DIE = 512,
	    PKT_C_OX_START = 600,
	    PKT_S_OX_START = 601,
	    PKT_S_OX_FINISH = 602,
	    PKT_C_OX_GET_HOST = 603,
	    PKT_S_OX_HOST = 604,
	    PKT_S_OX_ROUND_START = 605,
	    PKT_S_OX_ROUND_FINISH = 606,
	    PKT_S_OX_QUIZ = 607,
	    PKT_S_OX_DESTROY = 608,
	    PKT_C_OX_DIE = 609,
	}

	public void Register()
	{
        _onRecv.Add((ushort)MsgId.PKT_S_ENTER, MakePacket<S_ENTER>);
		_handler.Add((ushort)MsgId.PKT_S_ENTER, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_REENTER, MakePacket<S_REENTER>);
		_handler.Add((ushort)MsgId.PKT_S_REENTER, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_ADD_CLIENT, MakePacket<S_ADD_CLIENT>);
		_handler.Add((ushort)MsgId.PKT_S_ADD_CLIENT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_REMOVE_CLIENT, MakePacket<S_REMOVE_CLIENT>);
		_handler.Add((ushort)MsgId.PKT_S_REMOVE_CLIENT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_SET_NICKNAME, MakePacket<S_SET_NICKNAME>);
		_handler.Add((ushort)MsgId.PKT_S_SET_NICKNAME, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_SET_NICKNAME_NOTICE, MakePacket<S_SET_NICKNAME_NOTICE>);
		_handler.Add((ushort)MsgId.PKT_S_SET_NICKNAME_NOTICE, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_DISCONNECT, MakePacket<S_DISCONNECT>);
		_handler.Add((ushort)MsgId.PKT_S_DISCONNECT, new Dictionary<object, Action<PacketSession, IMessage>>());
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
        _onRecv.Add((ushort)MsgId.PKT_S_MYROOM_GET_ROOMINFO, MakePacket<S_MYROOM_GET_ROOMINFO>);
		_handler.Add((ushort)MsgId.PKT_S_MYROOM_GET_ROOMINFO, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MYROOM_SET_ROOMINFO, MakePacket<S_MYROOM_SET_ROOMINFO>);
		_handler.Add((ushort)MsgId.PKT_S_MYROOM_SET_ROOMINFO, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MYROOM_OTHER_ROOM_LIST, MakePacket<S_MYROOM_OTHER_ROOM_LIST>);
		_handler.Add((ushort)MsgId.PKT_S_MYROOM_OTHER_ROOM_LIST, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_ADD_WAITING_CLIENT, MakePacket<S_OFFICE_ADD_WAITING_CLIENT>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_ADD_WAITING_CLIENT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_REMOVE_WAITING_CLIENT, MakePacket<S_OFFICE_REMOVE_WAITING_CLIENT>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_REMOVE_WAITING_CLIENT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_ACCEPT_WAIT, MakePacket<S_OFFICE_ACCEPT_WAIT>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_ACCEPT_WAIT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_ACCEPT_WAIT_NOTICE, MakePacket<S_OFFICE_ACCEPT_WAIT_NOTICE>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_ACCEPT_WAIT_NOTICE, new Dictionary<object, Action<PacketSession, IMessage>>());
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
        _onRecv.Add((ushort)MsgId.PKT_S_OFFICE_SHARE, MakePacket<S_OFFICE_SHARE>);
		_handler.Add((ushort)MsgId.PKT_S_OFFICE_SHARE, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MATCHING_START, MakePacket<S_MATCHING_START>);
		_handler.Add((ushort)MsgId.PKT_S_MATCHING_START, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MATCHING_FINISH, MakePacket<S_MATCHING_FINISH>);
		_handler.Add((ushort)MsgId.PKT_S_MATCHING_FINISH, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MATCHING_HOST, MakePacket<S_MATCHING_HOST>);
		_handler.Add((ushort)MsgId.PKT_S_MATCHING_HOST, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MATCHING_ROUND_START, MakePacket<S_MATCHING_ROUND_START>);
		_handler.Add((ushort)MsgId.PKT_S_MATCHING_ROUND_START, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MATCHING_ROUND_FINISH, MakePacket<S_MATCHING_ROUND_FINISH>);
		_handler.Add((ushort)MsgId.PKT_S_MATCHING_ROUND_FINISH, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MATCHING_TILES, MakePacket<S_MATCHING_TILES>);
		_handler.Add((ushort)MsgId.PKT_S_MATCHING_TILES, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MATCHING_HINT, MakePacket<S_MATCHING_HINT>);
		_handler.Add((ushort)MsgId.PKT_S_MATCHING_HINT, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MATCHING_PROBLEM, MakePacket<S_MATCHING_PROBLEM>);
		_handler.Add((ushort)MsgId.PKT_S_MATCHING_PROBLEM, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MATCHING_DESTROY, MakePacket<S_MATCHING_DESTROY>);
		_handler.Add((ushort)MsgId.PKT_S_MATCHING_DESTROY, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_MATCHING_QUIZ_DISAPPEAR, MakePacket<S_MATCHING_QUIZ_DISAPPEAR>);
		_handler.Add((ushort)MsgId.PKT_S_MATCHING_QUIZ_DISAPPEAR, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OX_START, MakePacket<S_OX_START>);
		_handler.Add((ushort)MsgId.PKT_S_OX_START, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OX_FINISH, MakePacket<S_OX_FINISH>);
		_handler.Add((ushort)MsgId.PKT_S_OX_FINISH, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OX_HOST, MakePacket<S_OX_HOST>);
		_handler.Add((ushort)MsgId.PKT_S_OX_HOST, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OX_ROUND_START, MakePacket<S_OX_ROUND_START>);
		_handler.Add((ushort)MsgId.PKT_S_OX_ROUND_START, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OX_ROUND_FINISH, MakePacket<S_OX_ROUND_FINISH>);
		_handler.Add((ushort)MsgId.PKT_S_OX_ROUND_FINISH, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OX_QUIZ, MakePacket<S_OX_QUIZ>);
		_handler.Add((ushort)MsgId.PKT_S_OX_QUIZ, new Dictionary<object, Action<PacketSession, IMessage>>());
        _onRecv.Add((ushort)MsgId.PKT_S_OX_DESTROY, MakePacket<S_OX_DESTROY>);
		_handler.Add((ushort)MsgId.PKT_S_OX_DESTROY, new Dictionary<object, Action<PacketSession, IMessage>>());
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