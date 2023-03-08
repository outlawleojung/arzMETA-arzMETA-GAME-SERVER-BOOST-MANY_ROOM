#pragma once
#include "Acceptor.h"
#include "Session.h"

#include <set>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

using SessionFactory = function<shared_ptr<Session>(io_context& ioc)>;

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ip::address_v4 addr, unsigned short port, SessionFactory sessionFactory)
		:ioc(),
		ep(addr, port),
		_sessionFactory(sessionFactory)
	{}

	void Start();

	void AddSession(shared_ptr<Session> session);
	void ReleaseSession(shared_ptr<Session> session);

	io_context& GetIOC() { return ioc; }
	ip::tcp::endpoint GetEndPoint() { return ep; }

	shared_ptr<Session> CreateSession();

	bool Dispatch(unsigned int timeoutMs);

private:
	io_context ioc;
	ip::tcp::endpoint ep;
	shared_ptr<Acceptor> acceptor;
	SessionFactory _sessionFactory;
	set<shared_ptr<Session>> _sessions;
};