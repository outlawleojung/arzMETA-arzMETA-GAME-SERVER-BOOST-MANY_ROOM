#include "DummyClient.h"
#include "../pch.h"
#include "../PacketManager.h"
#include "../Client/Client.h"
#include "../Session/GameSession.h"

#include <boost/asio.hpp>

#include <sio_client.h>
#include <sio_socket.h>
#include <sio_message.h>

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

		shared_ptr<JobQueue> jobQueue = GPendingJobQueues->Pop();
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

void DoWorkerJob(io_context& ioc, bool& state)
{
	while (true)
	{
#ifdef linux
		LEndTickCount = GetTickCount() + WORKER_TICK;
#elif _WIN32
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;
#endif
		ioc.run_for(std::chrono::milliseconds{10});
		DistributeReservedJobs(); // JobTimer �� �ִ� Job ����
		DoGlobalQueueWork(); // GlobalQueue �� �ִ� JobQueue �� Job ����
	}
}

shared_ptr<Client> MakeClient(io_context& ioc, ip::tcp::endpoint& ep, string clientId)
{
	if (clients.count(clientId))
		return nullptr;

	auto session = make_shared<GameSession>(ioc);
	
	auto client = make_shared<Client>(clientId);
	client->session = session;
	session->owner = client;

	session->Connect(ep);

	return client;
}

int main()
{
	PacketManager::Init();

#ifdef linux
	struct ifaddrs* ifAddrStruct = NULL;
	struct ifaddrs* ifa = NULL;
	void* tmpAddrPtr = NULL;

	getifaddrs(&ifAddrStruct);

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr) {
			continue;
		}
		if (ifa->ifa_addr->sa_family == AF_INET) {
			tmpAddrPtr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			if (strcmp(ifa->ifa_name, "eth0") == 0)
			{
				localHostIp = addressBuffer;
			}
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
			}
		}
	}
#endif

	io_context ioc;
	ip::tcp::endpoint ep(ip::address_v4::from_string(localHostIp), tcpPort);
	//ip::tcp::endpoint ep(ip::address_v4::from_string("20.200.216.69"), 45456);

	bool state = true;

	for (int i = 0; i < 10; i++)
	{
		GThreadManager->Launch([&ioc, &state]()
			{
				while (state)
				{
					DoWorkerJob(ioc, state);
				}
			});
	}

	GLogManager->Log("enter \"help\" for information or enter \"exit\' to exit program");

	while (state)
	{
		string command;
		cin >> command;

		if (command == "exit")
		{
			state = false;
			continue;
		}

		if (command == "help")
		{
			GLogManager->Log("connect <client id> - Connect to server. And automatically send C_ENTER message");
			GLogManager->Log("leave <client id> - Send C_LEAVE message");
			GLogManager->Log("disconnect <client id> - Disconnect session");
			GLogManager->Log("move <client id> - Move game object by 1 in X direction. And send C_SET_TRANSFORM message");
		}

		if (command == "set")
		{
			string ip;
			cin >> ip;

			ep = ip::tcp::endpoint(ip::address_v4::from_string(ip), tcpPort);
		}

		if (command == "connect")
		{
			string clientId;
			cin >> clientId;
			string roomId;
			cin >> roomId;

			clientId = "Test_" + clientId;
			auto client = MakeClient(ioc, ep, clientId);
			if (client == nullptr)
			{
				GLogManager->Log("Duplicated Client Id : ", clientId);
				continue;
			}

			clients.insert({ client->clientId, client });

			Protocol::C_ENTER enter;
			enter.set_clientid(client->clientId);
			enter.set_roomid(roomId);
			client->Send(PacketManager::MakeSendBuffer(enter));

			continue;
		}

		if (command == "leave")
		{
			string clientId;
			cin >> clientId;

			auto client = clients.find(clientId);
			if (client == clients.end())
			{
				GLogManager->Log("Wrong Client Id : ", clientId);
				continue;
			}

			Protocol::C_LEAVE leave;
			client->second->Send(PacketManager::MakeSendBuffer(leave));

			continue;
		}

		if (command == "disconnect")
		{
			string clientId;
			cin >> clientId;

			auto client = clients.find(clientId);
			if (client == clients.end())
			{
				GLogManager->Log("Wrong Client Id : ", clientId);
				continue;
			}

			client->second->session->Disconnect();

			continue;
		}

		else if (command._Equal("move"))
		{
			string clientId;
			cin >> clientId;

			auto client = clients.find(clientId);
			if (client == clients.end())
			{
				GLogManager->Log("Wrong Client Id : ", clientId);
				continue;
			}
			
			Protocol::C_BASE_SET_TRANSFORM setTransform;
			auto position = setTransform.mutable_position();
			
			client->second->position_x++;
			position->set_x(client->second->position_x);
			position->set_y(client->second->position_y);
			position->set_z(client->second->position_z);
			
			auto rotation = setTransform.mutable_rotation();
			rotation->set_x(0);
			rotation->set_y(0);
			rotation->set_z(0);
			
			setTransform.set_objectid(client->second->objectId);

			client->second->Send(PacketManager::MakeSendBuffer(setTransform));
		
			continue;
		}

		else if (command._Equal("connect_many"))
		{
			static int idGenerator = 0;
			int count;
			string roomId;
			float x, y, z;
			cin >> count >> roomId >> x >> y >> z;

			y = 0.45f;

			for (int i = 0; i < count; i++)
			{
				string clientId = "Test_" + std::to_string(idGenerator++);

				auto client = MakeClient(ioc, ep, clientId);
				if (client == nullptr)
				{
					GLogManager->Log("Duplicated Client Id : ", clientId);
					continue;
				}
				clients.insert({ client->clientId, client });
				client->start_x = x;
				client->start_y = y;
				client->start_z = z;
				client->position_x = x;
				client->position_y = y;
				client->position_z = z;
				Protocol::C_ENTER enter;
				enter.set_clientid(client->clientId);
				enter.set_roomid(roomId);
				client->Send(PacketManager::MakeSendBuffer(enter));
			}
			continue;
		}
	}

	GThreadManager->Join();
}