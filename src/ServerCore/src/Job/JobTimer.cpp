#include "../Main/pch.h"
#include "JobTimer.h"
#include "JobQueue.h"

void JobTimer::Reserve(unsigned long long tickAfter, weak_ptr<JobQueue> owner, shared_ptr<Job> job)
{
#ifdef linux
	const unsigned long long executeTick = GetTickCount() + tickAfter;
#elif _WIN32
	const unsigned long long executeTick = ::GetTickCount64() + tickAfter;
#endif
	//JobData* jobData = ObjectPool<JobData>::Pop(owner, job);
	JobData* jobData = new JobData(owner, job);

	boost::unique_lock<boost::recursive_mutex> lock(_mtx);

	_items.push(TimerItem{ executeTick, jobData });
}

void JobTimer::Distribute(unsigned long long now)
{
	// 한 번에 1 쓰레드만 통과
	if (_distributing.exchange(true) == true)
		return;

	vector<TimerItem> items;

	{
		boost::unique_lock<boost::recursive_mutex> lock(_mtx);

		while (_items.empty() == false)
		{
			const TimerItem& timerItem = _items.top();
			if (now < timerItem.executeTick)
				break;

			items.push_back(timerItem);
			_items.pop();
		}
	}

	for (TimerItem& item : items)
	{
		if (shared_ptr<JobQueue> owner = item.jobData->owner.lock())
			owner->Push(item.jobData->job);

		//ObjectPool<JobData>::Push(item.jobData);
		delete item.jobData;
	}

	// 끝났으면 풀어준다
	_distributing.store(false);
}

void JobTimer::Clear()
{
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);

	while (_items.empty() == false)
	{
		const TimerItem& timerItem = _items.top();
		//ObjectPool<JobData>::Push(timerItem.jobData);
		 delete timerItem.jobData;
		_items.pop();
	}
}