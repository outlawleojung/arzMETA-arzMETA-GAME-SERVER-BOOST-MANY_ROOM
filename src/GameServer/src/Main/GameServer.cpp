﻿#include <boost/asio.hpp>
#include "httplib.h"

#include "GameServer.h"
#include "../Util/pch.h"
#include "../ClientPacketHandler.h"
#include "../Session/GameSession.h"
#include "../Room/RoomManager.h"
#include "../Room/RoomBase.h"
#include "../Room/OfficeRoom.h"
#include "../HTTP/HttpServer.h"

#ifdef linux
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#endif

using namespace std;
using namespace boost::asio;

enum
{
	WORKER_TICK = 64
};

void DoGlobalQueueWork()
{
	while (true)
	{
#ifdef linux
		 unsigned long long now = GetTickCount();
#elif _WIN32
		 unsigned long long now = ::GetTickCount64();
#endif
		if (now > LEndTickCount)
			break;

		shared_ptr<JobQueue> jobQueue = GGlobalQueue->Pop();
		if (jobQueue == nullptr)
			break;

		jobQueue->Execute();
	}
}

void DistributeReservedJobs()
{
#ifdef linux
	unsigned long long now = GetTickCount();
#elif _WIN32
	unsigned long long now = ::GetTickCount64();
#endif

	GJobTimer->Distribute(now);
}

void DoWorkerJob(shared_ptr<Service>& service)
{
	int count = 0;

	while (true)
	{
#ifdef linux
		LEndTickCount = GetTickCount() + WORKER_TICK;
#elif _WIN32
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;
#endif
		service->Dispatch(10); // IO 수행
		DistributeReservedJobs(); // JobTimer 에 있는 Job 수행
		DoGlobalQueueWork(); // GlobalQueue 에 있는 JobQueue 의 Job 수행
	}
}

int main()
{
	ClientPacketHandler::Init();

	string localHostIp;

#ifdef linux
	struct ifaddrs* ifAddrStruct = NULL;
	struct ifaddrs* ifa = NULL;
	void* tmpAddrPtr = NULL;

	getifaddrs(&ifAddrStruct);

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr) {
			continue;
		}
		if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
			// is a valid IP4 Address
			tmpAddrPtr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			if (strcmp(ifa->ifa_name, "eth0") == 0)
			{
				localHostIp = addressBuffer;
			}
			//printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer);
		}
	}
	if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);
#elif _WIN32
	{
		boost::asio::io_service io_service;
		boost::asio::ip::tcp::resolver resolver(io_service);
		boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
		boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
		boost::asio::ip::tcp::resolver::iterator end;
		while (iter != end) {
			boost::asio::ip::tcp::endpoint ep = *iter++;
			if (ep.protocol() == boost::asio::ip::tcp::v4() && ep.address().to_string() != "127.0.0.1") {
				localHostIp = ep.address().to_string();
				break;
				//std::cout << "Local IP address: " << ep.address().to_string() << std::endl;
			}
		}
	}
#endif

	GLogManager->Log("Local Host IP : ", localHostIp);

	if (localHostIp.empty())
		return 0;

	shared_ptr<Service> service = make_shared<Service>(
		ip::address_v4::from_string(localHostIp),
		45456,
		[](io_context& ioc) {
			return make_shared<GameSession>(ioc);
		}
		);

	service->Start();

	GLogManager->Log("Game Server Started with IP ", localHostIp);

	for (int i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				DoWorkerJob(service);
			});
	}

	GThreadManager->Launch([localHostIp]()
		{
			HttpServer server;
			server.start(localHostIp, 45457);
		});

	//시작 처리

	GThreadManager->Join();

	//종료 처리
}
