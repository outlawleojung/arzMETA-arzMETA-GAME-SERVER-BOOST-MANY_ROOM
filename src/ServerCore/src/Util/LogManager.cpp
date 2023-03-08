#include "LogManager.h"

void LogManager::PrintLog(shared_ptr<LogObj> log)
{
	std::cout << log->content << std::endl;
}