#pragma once

#include "../Main/pch.h"

#include <chrono>
#include <ctime>
#include <iomanip>

class LogObj
{
public:
	LogObj()
		:size(0)
	{
	}

	void Init()
	{
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm bt;

#ifdef linux
		localtime_r(&t, &bt);
#elif _WIN32
		localtime_s(&bt, &t);
#endif

		strftime(content, 12, "[%H:%M:%S] ", &bt);

		size = 11;
	}

	bool Add(const string& s)
	{
		try
		{
#ifdef linux
			strcpy(&content[size], s.c_str());
#elif _WIN32
			strcpy_s(&content[size], s.size() + 1, s.c_str());
#endif
		}
		catch (...)
		{
			return false;
		}
		size += s.size();

		return true;
	}

public:
	char content[200];
	int size;
};

class LogManager : public JobQueue
{
private:
	void AppendToString(shared_ptr<LogObj> log) { return; }

	template <typename String, typename... Strings>
	void AppendToString(shared_ptr<LogObj> log, const String& s, Strings... strs) {
		log->Add(s);
		AppendToString(log, strs...);
	}

public:
	template <typename String, typename... Strings>
	void Log(const String& s, Strings... strs)
	{
		auto log = make_shared<LogObj>();
		log->Init();
		AppendToString(log, s, strs...);

		DoAsync(&LogManager::PrintLog, log);
	}

	void PrintLog(shared_ptr<LogObj> log);
};