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
{%- for pkt in parser.total_pkt %}
	    PKT_{{pkt.name}} = {{pkt.id}},
{%- endfor %}
	}

	public void Register()
	{
{%- for pkt in parser.send_pkt %}
        _onRecv.Add((ushort)MsgId.PKT_{{pkt.name}}, MakePacket<{{pkt.name}}>);
		_handler.Add((ushort)MsgId.PKT_{{pkt.name}}, new Dictionary<object, Action<PacketSession, IMessage>>());
{%- endfor %}
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