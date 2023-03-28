#include "Session.h"
#include "Service.h"
#include "../Main/pch.h"

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

using namespace std;
using namespace boost::asio;

Session::Session(io_context& context) : _socket(make_shared<ip::tcp::socket>(context)),
_recvBuffer(BUFFER_SIZE)
{
}

Session::~Session()
{
	GLogManager->Log("Socket Destroyed");
}

void Session::Disconnect()
{
	if (_isConnected.exchange(false) == false)
		return;

	_socket->close();
	_service->ReleaseSession(shared_from_this());

	OnDisconnected();
}

void Session::ProcessConnect()
{
	boost::asio::socket_base::linger option(true, 100);
	_socket->set_option(option);

	_isConnected.store(true);
	_service->AddSession(shared_from_this());
	OnConnected();
	RegisterRecv();
}

void Session::RegisterRecv()
{
	mutable_buffer buffer(reinterpret_cast<char*>(_recvBuffer.WritePos()), _recvBuffer.FreeSize());

	auto ref = shared_from_this();

	_socket->async_receive(buffer, [this, ref](const boost::system::error_code& error, std::size_t bytes_transferred) {

		if (bytes_transferred == 0)
		{
			Disconnect();
			return;
		}

		if (_recvBuffer.OnWrite(bytes_transferred) == false)
		{
			Disconnect();
			return;
		}

		int dataSize = _recvBuffer.DataSize();
		int processLen = OnRecv(_recvBuffer.ReadPos(), dataSize);
		if (processLen < 0 || dataSize < processLen || _recvBuffer.OnRead(processLen) == false)
		{
			Disconnect();
			return;
		}

		_recvBuffer.Clean();

		RegisterRecv();
		});
}

void Session::Send(std::shared_ptr<SendBuffer> sendBuffer)
{
	bool registerSend = false;

	{
		lock_guard<recursive_mutex> lock(_mtx);

		_sendQueue.push(sendBuffer);

		if (_isSendRegistered.exchange(true) == false)
			registerSend = true;
	}

	if (registerSend)
		RegisterSend();
}

void Session::SendMany(std::shared_ptr<std::vector<std::shared_ptr<SendBuffer>>> sendBuffers)
{
	bool registerSend = false;

	{
		lock_guard<recursive_mutex> lock(_mtx);

		for (auto it = sendBuffers->begin(); it != sendBuffers->end(); it++)
			_sendQueue.push(*it);

		if (_isSendRegistered.exchange(true) == false)
			registerSend = true;
	}

	if (registerSend)
		RegisterSend();
}

void Session::RegisterSend()
{
	//shared_ptr<SendBuffer>s 를 이용해 sendbuffer 의 레퍼런스 관리, send 작업이 완료될 때까지 버퍼의 내용이 사라지지 않도록 한다
	{
		lock_guard<recursive_mutex> lock(_mtx);

		sendBufferRefs.reserve(_sendQueue.size());

		while (_sendQueue.empty() == false)
		{
			sendBufferRefs.emplace_back(_sendQueue.front());
			_sendQueue.pop();
		}
	}

	vector<boost::asio::const_buffer> sendBuffers = vector<boost::asio::const_buffer>();
	
	for (shared_ptr<SendBuffer> sendBuffer : sendBufferRefs)
		sendBuffers.emplace_back(sendBuffer->Buffer(), sendBuffer->WriteSize());

	auto ref = shared_from_this();

	_socket->async_send(sendBuffers, [this, ref](const boost::system::error_code& error, std::size_t bytes_transferred) {
		
		sendBufferRefs.clear();

		if (!_isConnected)
			return;

		if (error)
			return;

		if (bytes_transferred == 0)
		{
			Disconnect();
			return;
		}

		{
			lock_guard<recursive_mutex> lock(_mtx);
			if (_sendQueue.empty())
				_isSendRegistered.store(false);
			else
				RegisterSend();
		}

		});
}

int PacketSession::OnRecv(unsigned char* buffer, int len)
{
	int processLen = 0;

	while (true)
	{
		int dataSize = len - processLen;
		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));
		if (dataSize < header.size)
			break;

		OnRecvPacket(&buffer[processLen], header.size);

		processLen += header.size;
	}

	return processLen;
}