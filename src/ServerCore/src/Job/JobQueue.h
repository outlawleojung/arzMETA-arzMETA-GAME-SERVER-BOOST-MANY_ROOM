#pragma once

#include <atomic>

#include "../Main/pch.h"
#include "../Main/Singleton.h"
#include "Job.h"
#include "LockQueue.h"
#include "JobTimer.h"

class JobQueue : public enable_shared_from_this<JobQueue>
{
public:
	void DoAsync(CallbackType&& callback)
	{
		//Push(ObjectPool<Job>::MakeShared(std::move(callback)));
		Push(make_shared<Job>(std::move(callback)));
	}

	template<typename T, typename Ret, typename... Args>
	void DoAsync(Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		//Push(ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...));
		Push(make_shared<Job>(owner, memFunc, std::forward<Args>(args)...));
	}

	template<typename T, typename Ret, typename... Args>
	void DoAsyncPushOnly(Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		//Push(ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...), true);
		Push(make_shared<Job>(owner, memFunc, std::forward<Args>(args)...), true);
	}

	void DoTimer(unsigned long long tickAfter, CallbackType&& callback)
	{
		//shared_ptr<Job> job = ObjectPool<Job>::MakeShared(std::move(callback));
		shared_ptr<Job> job = make_shared<Job>(std::move(callback));
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	template<typename T, typename Ret, typename... Args>
	void DoTimer(unsigned long long tickAfter, Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		//shared_ptr<Job> job = ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...);
		shared_ptr<Job> job = make_shared<Job>(owner, memFunc, std::forward<Args>(args)...);
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	void					ClearJobs() { _jobs.Clear(); }

public:
	void Push(shared_ptr<Job> job, bool pushOnly = false);
	void Execute();

protected:
	LockQueue<shared_ptr<Job>> _jobs;
	atomic<int> _jobCount = { 0 };
};