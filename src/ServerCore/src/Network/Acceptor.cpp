#include <boost/asio.hpp>
#include <iostream>

#include "Acceptor.h"
#include "Service.h"

using namespace std;
using namespace boost::asio;

bool Acceptor::StartAccept(shared_ptr<Service> owner)
{
	_owner = owner;

	acceptor = make_shared<ip::tcp::acceptor>(_owner->GetIOC(), _owner->GetEndPoint().protocol());
	acceptor->bind(_owner->GetEndPoint());
	acceptor->listen(); // 기본값인 MAX_LISTEN_CONNECTION 사용

	// RegisterAccept 를 여러 번 호출해야 하나? 그래야 동시에 복수의 세션을 받을 수 있나?
	// 지금은 한 번에 한 개의 세션만 받을 수 있는 것 같은데?
	// 여기에서 호출한 RegisterAccept 의 수만큼 동시에 세션을 받을 수 있게 되는 것?
	RegisterAccept();

	return true;
}

void Acceptor::RegisterAccept()
{
	auto session = _owner->CreateSession();
	acceptor->async_accept(*session->GetSocket(), [this, session](const boost::system::error_code& error) {
		if (error)
			return;

		session->ProcessConnect();
		this->RegisterAccept();
		});
}