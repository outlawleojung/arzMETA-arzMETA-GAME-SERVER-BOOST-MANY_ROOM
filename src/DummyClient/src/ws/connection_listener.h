#pragma once

#include <sio_client.h>
#include <mutex>
#include <condition_variable>
#include <iostream>

class connection_listener
{
public:
	connection_listener(
		sio::client& h,
		std::mutex& l,
		std::condition_variable_any& c,
		bool& cf
	) :
		handler(h),
		_lock(l),
		_cond(c),
		connect_finish(cf)
	{}

	void on_connected()
	{
		_lock.lock();
		_cond.notify_all();
		connect_finish = true;
		_lock.unlock();
	}

	void on_close(sio::client::close_reason const& reason)
	{
		std::cout << "sio closed " << std::endl;
		exit(0);
	}

	void on_fail()
	{
		std::cout << "sio failed " << std::endl;
		exit(0);
	}

private:
	sio::client& handler;
	std::mutex& _lock;
	std::condition_variable_any& _cond;
	bool& connect_finish;
};