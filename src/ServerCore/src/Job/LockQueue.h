#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <memory>

using namespace std;

template<typename T>
class LockQueue
{
public:
	void Push(T item)
	{
		lock_guard<recursive_mutex> lock(_mtx);
		_items.push(item);
	}

	T Pop()
	{
		lock_guard<recursive_mutex> lock(_mtx);
		if (_items.empty())
			return T();

		T ret = _items.front();
		_items.pop();
		return ret;
	}

	void PopAll(std::vector<T>& items)
	{
		lock_guard<recursive_mutex> lock(_mtx);
		while (T item = Pop())
			items.push_back(item);
	}

	void Clear()
	{
		lock_guard<recursive_mutex> lock(_mtx);
		_items = queue<T>();
	}

private:
	recursive_mutex _mtx;
	queue<T> _items;
};