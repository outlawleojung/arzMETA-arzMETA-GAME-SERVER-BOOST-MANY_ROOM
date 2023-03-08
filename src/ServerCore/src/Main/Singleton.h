#pragma once

extern class ThreadManager* GThreadManager;
extern class SendBufferManager* GSendBufferManager;
extern class GlobalQueue* GGlobalQueue;
extern class JobTimer* GJobTimer;

extern bool isRunning;

#include <memory>
extern std::shared_ptr<class LogManager> GLogManager;