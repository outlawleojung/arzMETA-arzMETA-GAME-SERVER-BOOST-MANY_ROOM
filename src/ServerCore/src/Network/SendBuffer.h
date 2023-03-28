#pragma once

#include <array>
#include <vector>
#include <boost/thread.hpp>

using namespace std;

class SendBufferChunk;

class SendBuffer
{
public:
	SendBuffer(shared_ptr<SendBufferChunk> owner, unsigned char* buffer, unsigned int allocSize);
	~SendBuffer();

	unsigned char* Buffer() { return _buffer; }
	unsigned int AllocSize() { return _allocSize; }
	unsigned int WriteSize() { return _writeSize; }
	void Close(unsigned int writeSize);

private:
	unsigned char* _buffer;
	unsigned int _allocSize = 0;
	unsigned int _writeSize = 0;
	shared_ptr<SendBufferChunk>	_owner;
};

class SendBufferChunk : public enable_shared_from_this<SendBufferChunk>
{
	enum
	{
		SEND_BUFFER_CHUNK_SIZE = 6000
	};

public:
	SendBufferChunk();
	~SendBufferChunk();

	void Reset();
	shared_ptr<SendBuffer> Open(unsigned int allocSize);
	void Close(unsigned int writeSize);

	bool IsOpen() { return _open; }
	unsigned char* Buffer() { return &_buffer[_usedSize]; }
	unsigned int FreeSize() { return static_cast<unsigned int>(_buffer.size()) - _usedSize; }

private:
	array<unsigned char, SEND_BUFFER_CHUNK_SIZE> _buffer = {};
	bool _open = false;
	unsigned int _usedSize = 0;
};

class SendBufferManager
{
public:
	shared_ptr<SendBuffer> Open(unsigned int size);

private:
	shared_ptr<SendBufferChunk>	Pop();
	void Push(shared_ptr<SendBufferChunk> buffer);
	static void PushGlobal(SendBufferChunk* buffer);

private:
	mutex _mtx;
	vector<shared_ptr<SendBufferChunk>> _sendBufferChunks;
};
