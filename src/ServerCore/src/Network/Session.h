#pragma once

#include "RecvBuffer.h"
#include "SendBuffer.h"

#include <queue>
#include <atomic>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace boost::asio;

class Service;

class Session : public enable_shared_from_this<Session>
{
	enum
	{
		BUFFER_SIZE = 0x10000, // 64KB
	};

public:
	Session(io_context& context);
	virtual ~Session();

	void SerService(shared_ptr<Service> service) { _service = service; }

	shared_ptr<ip::tcp::socket> GetSocket() { return _socket; }

	void Disconnect();

	void Send(std::shared_ptr<SendBuffer> sendBuffer);
	void SendMany(std::shared_ptr<std::vector<std::shared_ptr<SendBuffer>>> sendBuffers);
	
	void RegisterRecv();
	void RegisterSend();

	void ProcessConnect();

	virtual void OnConnected() {};

	bool IsConnected() { return _isConnected; }

protected:
	virtual void OnDisconnected() {};
	virtual int OnRecv(unsigned char* buffer, int len) { return len; }

private:
	shared_ptr<ip::tcp::socket> _socket;
	shared_ptr<Service> _service;
	RecvBuffer _recvBuffer;
	queue<shared_ptr<SendBuffer>> _sendQueue;
	vector<shared_ptr<SendBuffer>> sendBufferRefs;
	atomic<bool> _isSendRegistered = { false };
	atomic<bool> _isConnected = { false };

	boost::recursive_mutex _mtx;
};

struct PacketHeader
{
	unsigned short size;
	unsigned short id;
};

class PacketSession : public Session
{
public:
	PacketSession(io_context& ioc) : Session(ioc)
	{}
	virtual ~PacketSession() {};

	virtual int OnRecv(unsigned char* buffer, int len) final; //sealed;
	virtual void OnRecvPacket(unsigned char* buffer, int len) = 0; //abstract;
};