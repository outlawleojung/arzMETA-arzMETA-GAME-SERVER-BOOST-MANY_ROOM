#include "Client.h"
#include "../Session/GameSession.h"
#include "../Protocols.h"
#include "../PacketManager.h"

void Client::Send(shared_ptr<SendBuffer> sendBuffer)
{
	session->Send(sendBuffer);
}

void Client::OnDisconnected()
{
	clients.erase(clientId);
	session = nullptr;

	CloseWS();
}

void Client::StartMove()
{
	//randomly choose to move or stop
	if (rand() % 2 == 0)
	{
		if (isMoving)
		{
			isMoving = false;
			auto msg = std::static_pointer_cast<sio::object_message>(sio::object_message::create());
			msg->insert("animationId", "Avatar");
			msg->insert("animation", "M9AzAAIFA1QKCA0B");
			sio::message::list list;
			list.push(msg);
			h.socket("/test")->emit("C_Test_Animation", list);
		}

		DoTimer(200, &Client::StartMove);
	}
	else
	{
		if (!isMoving)
		{
			isMoving = true;
			auto msg = std::static_pointer_cast<sio::object_message>(sio::object_message::create());
			msg->insert("animationId", "Avatar");
			msg->insert("animation", "M9MzAAIFA1QKCA0B");
			sio::message::list list;
			list.push(msg);
			h.socket("/test")->emit("C_Test_Animation", list);
		}

		position_x += (rand() % 3) - 1;
		position_z += (rand() % 3) - 1;

		if(position_x > start_x + cap)
			position_x = start_x + cap;
		else if(position_x < start_x - cap)
			position_x = start_x - cap;

		if (position_z > start_z + cap)
			position_z = start_z + cap;
		else if (position_z < start_z - cap)
			position_z = start_z - cap;

		{
			auto msg = std::static_pointer_cast<sio::object_message>(sio::object_message::create());
			msg->insert("px", to_string(position_x));
			msg->insert("py", to_string(position_y));
			msg->insert("pz", to_string(position_z));
			msg->insert("euler", "0.0000");
			sio::message::list list;
			list.push(msg);
			h.socket("/test")->emit("C_Test_Transform", list);
		}

		DoTimer(200, &Client::StartMove);
	}
}

#include "../ws/connection_listener.h"
void Client::ConnectWS()
{
	l = std::make_shared<connection_listener>(h, _lock, _cond, connect_finish);

	h.set_open_listener(std::bind(&connection_listener::on_connected, *l));
	h.set_close_listener(std::bind(&connection_listener::on_close, *l, std::placeholders::_1));
	h.set_fail_listener(std::bind(&connection_listener::on_fail, *l));
	
	auto auth = std::static_pointer_cast<sio::object_message>(sio::object_message::create());
	//auth->insert("jwtAccessToken", "testToken");
	auth->insert("sessionId", "testSessionId");

	h.connect("http://192.168.0.104:3910", auth);

	_lock.lock();
	if (!connect_finish)
	{
		_cond.wait(_lock);
	}
	_lock.unlock();

	Register();
}

void Client::CloseWS()
{
	h.socket("/test")->off_all();
	h.close();
}

#include <nlohmann/json.hpp>
void Client::Register()
{
	h.socket("/test")->on("S_Test_EnterRoom_Notice",
		sio::socket::event_listener_aux([&](string const& name, sio::message::ptr const& data, bool isAck, sio::message::list& ack_resp) {
			_lock.lock();

			auto res = nlohmann::json::parse(data->get_string());
			auto msg = std::static_pointer_cast<sio::object_message>(sio::object_message::create());
			msg->insert("receiverId", res["clientId"].get<string>());
			sio::message::list list;
			h.socket("/test")->emit("C_Test_EnterRoom_Notice", list);

			_cond.notify_all();
			_lock.unlock();
			}));

	h.socket("/test")->on("S_Test_Instantiate",
		sio::socket::event_listener_aux([&](string const& name, sio::message::ptr const& data, bool isAck, sio::message::list& ack_resp) {
			_lock.lock();

			StartMove();

			_cond.notify_all();
			_lock.unlock();
			}));
}

void Client::TestSend()
{
	auto test_msg = std::static_pointer_cast<sio::object_message>(sio::object_message::create());
	test_msg->insert("message", "thisistest");
	sio::message::list list;
	list.push(test_msg);
	h.socket("/test")->emit("C_Test", list);
}

void Client::SendWS(string name, sio::message::list list)
{
	h.socket("/test")->emit(name, list);
}
