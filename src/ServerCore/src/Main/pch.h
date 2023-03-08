#pragma once

#include "../Thread/TLS.h"
#include "../Thread/ThreadManager.h"

#include "../Network/Service.h"
#include "../Network/SendBuffer.h"

#include "../Job/JobQueue.h"
#include "../Job/JobTimer.h"
#include "../Job/GlobalQueue.h"

#include "../Util/LogManager.h"

#include "Singleton.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <condition_variable>

//extern condition_variable isProcessRunning;

#ifdef linux
#include <sys/time.h>
static unsigned long long GetTickCount()
{
	struct timeval  tv;
	gettimeofday(&tv, NULL);

	return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}
#endif