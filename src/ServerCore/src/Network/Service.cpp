#include "Service.h"

#include <chrono>

using namespace std;

void Service::Start()
{
	acceptor = make_shared<Acceptor>();
	acceptor->StartAccept(shared_from_this());
}

shared_ptr<Session> Service::CreateSession()
{
	auto session = _sessionFactory(ioc);
	session->SerService(shared_from_this());
	return session;
}

bool Service::Dispatch(unsigned int timeoutMs)
{
	ioc.run_for(std::chrono::milliseconds{timeoutMs});
	return true;
}

void Service::AddSession(shared_ptr<Session> session)
{
	_sessions.insert(session);
}

void Service::ReleaseSession(shared_ptr<Session> session)
{
	_sessions.erase(session);
}
