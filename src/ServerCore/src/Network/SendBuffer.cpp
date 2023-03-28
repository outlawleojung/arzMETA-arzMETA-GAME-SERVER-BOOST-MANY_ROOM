#include "../Main/pch.h"
#include "SendBuffer.h"
//#include "../Memory/ObjectPool.h"

using namespace std;

SendBuffer::SendBuffer(shared_ptr<SendBufferChunk> owner, unsigned char* buffer, unsigned int allocSize)
	: _owner(owner), _buffer(buffer), _allocSize(allocSize)
{
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Close(unsigned int writeSize)
{
	_writeSize = writeSize;
	_owner->Close(writeSize);
}

SendBufferChunk::SendBufferChunk()
{
}

SendBufferChunk::~SendBufferChunk()
{
}

void SendBufferChunk::Reset()
{
	_open = false;
	_usedSize = 0;
}

shared_ptr<SendBuffer> SendBufferChunk::Open(unsigned int allocSize)
{
	if (allocSize > FreeSize())
		return nullptr;

	_open = true;
	
	return make_shared<SendBuffer>(shared_from_this(), Buffer(), allocSize);
}

void SendBufferChunk::Close(unsigned int writeSize)
{
	_open = false;
	_usedSize += writeSize;
}

shared_ptr<SendBuffer> SendBufferManager::Open(unsigned int size)
{
	if (LSendBufferChunk == nullptr)
	{
		LSendBufferChunk = Pop();
		LSendBufferChunk->Reset();
	}

	if (LSendBufferChunk->FreeSize() < size)
	{
		LSendBufferChunk = Pop();
		LSendBufferChunk->Reset();
	}

	return LSendBufferChunk->Open(size);
}

shared_ptr<SendBufferChunk> SendBufferManager::Pop()
{
	{
		lock_guard<mutex> lock(_mtx);
		if (_sendBufferChunks.empty() == false)
		{
			shared_ptr<SendBufferChunk> sendBufferChunk = _sendBufferChunks.back();
			_sendBufferChunks.pop_back();
			return sendBufferChunk;
		}
	}

	return shared_ptr<SendBufferChunk>(new SendBufferChunk, PushGlobal);
}

void SendBufferManager::Push(shared_ptr<SendBufferChunk> buffer)
{
	lock_guard<mutex> lock(_mtx);
	_sendBufferChunks.push_back(buffer);
}

void SendBufferManager::PushGlobal(SendBufferChunk* buffer)
{
	GSendBufferManager->Push(shared_ptr<SendBufferChunk>(buffer, PushGlobal));
}