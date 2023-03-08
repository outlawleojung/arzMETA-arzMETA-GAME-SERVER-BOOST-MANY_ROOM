#pragma once

#include <queue>
#include <vector>
#include <boost/thread.hpp>

using namespace std;

template<typename T>
class LockQueue
{
public:
	void Push(T item)
	{
		//boost::unique_lock<boost::shared_mutex> lock(_mtx);
		boost::unique_lock<boost::recursive_mutex> lock(_mtx);
		_items.push(item);
	}

	T Pop()
	{
		//boost::unique_lock<boost::shared_mutex> lock(_mtx);
		boost::unique_lock<boost::recursive_mutex> lock(_mtx);
		if (_items.empty())
			return T();

		T ret = _items.front();
		_items.pop();
		return ret;
	}

	void PopAll(std::vector<T>& items)
	{
		//boost::unique_lock<boost::shared_mutex> lock(_mtx);
		boost::unique_lock<boost::recursive_mutex> lock(_mtx);
		while (T item = Pop())
			items.push_back(item);
	}

	void Clear()
	{
		//boost::unique_lock<boost::shared_mutex> lock(_mtx);
		boost::unique_lock<boost::recursive_mutex> lock(_mtx);
		_items = queue<T>();
	}

private:
	//boost::shared_mutex _mtx;
	boost::recursive_mutex _mtx;
	queue<T> _items;
};