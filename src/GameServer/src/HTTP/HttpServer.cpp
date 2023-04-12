#include "HttpServer.h"
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "../GameContents/GameContents.h"

void HttpServer::start(string ip, int port)
{
    httplib::Server svr;

    svr.Get("/Rooms", [](const httplib::Request& req, httplib::Response& res) {

		map<string, string> query;

		for (auto param = req.params.begin(); param != req.params.end(); param++)
			query[param->first] = param->second;

        res.set_content(GRoomManager->GetRoom(query).dump(), "text/plain");
        });

	svr.Post("/Rooms", [](const httplib::Request& req, httplib::Response& res) {

		map<string, string> query;

		for (auto param = req.params.begin(); param != req.params.end(); param++)
			query[param->first] = param->second;

		res.set_content(GRoomManager->GetRoom(query).dump(), "text/plain");
		});

    svr.Post("/MakeRoom", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {

        std::string bodyStr;
        content_reader([&](const char* data, size_t data_length) {
            bodyStr.append(data, data_length);
            return true;
            });

        nlohmann::json body = nlohmann::json::parse(bodyStr);

		cout << body.dump() << endl;

		vector<string> scenes;
		
		shared_ptr<RoomBase> room;

		if (body["roomType"] == "Office")
		{
			scenes.push_back("Office");

			room = make_shared<OfficeRoom>(scenes);

			room->type = RoomType::Office;

			static_pointer_cast<OfficeRoom>(room)->roomCode = body["roomCode"];
			static_pointer_cast<OfficeRoom>(room)->roomName = body["roomName"];
			static_pointer_cast<OfficeRoom>(room)->description = body["description"];
			static_pointer_cast<OfficeRoom>(room)->spaceInfoId = body["spaceInfoId"];
			static_pointer_cast<OfficeRoom>(room)->thumbnail = body["thumbnail"];
			static_pointer_cast<OfficeRoom>(room)->modeType = body["modeType"];
			static_pointer_cast<OfficeRoom>(room)->topicType = body["topicType"];
			static_pointer_cast<OfficeRoom>(room)->creatorId = body["creatorId"];
			
			static_pointer_cast<OfficeRoom>(room)->password = body["password"];
			if (body["password"] == "")
				static_pointer_cast<OfficeRoom>(room)->isPassword = false;
			else
				static_pointer_cast<OfficeRoom>(room)->isPassword = true;

			static_pointer_cast<OfficeRoom>(room)->maxPlayerNumber = body["personnel"];
			static_pointer_cast<OfficeRoom>(room)->observer = body["observer"];
			static_pointer_cast<OfficeRoom>(room)->isWaitingRoom = body["isWaitingRoom"];
			static_pointer_cast<OfficeRoom>(room)->isAdvertising = body["isAdvertising"];
			static_pointer_cast<OfficeRoom>(room)->runningTime = body["runningTime"];

			static_pointer_cast<OfficeRoom>(room)->AUTO_DESTROY = true;
			static_pointer_cast<OfficeRoom>(room)->DESTROY_WHEN_EMPTY = true;

		}
		else if(body["roomType"] == "MyRoom")
		{
			map<string, string> query;
			query.insert({ "type", "MyRoom" });
			query.insert({ "ownerId", body["ownerId"]});
			if (GRoomManager->GetRoom(query).size() > 0)
			{
				nlohmann::json resJson;
				resJson["result"] = "MYROOM_ALREADY_EXISTS";
				res.set_content(resJson.dump(), "application/json");
				return;
			}

			scenes.push_back("MyRoom");

			room = make_shared<MyRoomRoom>(scenes);

			room->type = RoomType::MyRoom;

			static_pointer_cast<MyRoomRoom>(room)->ownerId = body["ownerId"];
		}
		else if (body["roomType"] == "ArzLand")
		{
			scenes.push_back("ArzLand");
			scenes.push_back("GameZone");
			scenes.push_back("VoteZone");
			scenes.push_back("CTFZone");

			room = make_shared<GameRoom>(scenes);

			room->type = RoomType::ArzLand;
		}
		else if (body["roomType"] == "JumpingMatching")
		{
			scenes.push_back("JumpingMatching");
			
			room = make_shared<MatchingRoom>(scenes);
			
			room->type = RoomType::Matching;

			room->maxPlayerNumber = body["maxPlayerNumber"];
			static_pointer_cast<MatchingRoom>(room)->roomName = body["roomName"];
		}
		else if (body["roomType"] == "OX")
		{
			scenes.push_back("OX");

			room = make_shared<OXRoom>(scenes);

			room->type = RoomType::OX;

			room->maxPlayerNumber = body["maxPlayerNumber"];
			static_pointer_cast<OXRoom>(room)->roomName = body["roomName"];
		}
		else
		{
			nlohmann::json resJson;
			resJson["result"] = "WRONG_ROOM_TYPE";
			
			res.set_content(resJson.dump(), "application/json");
			return;
		}

		GRoomManager->AddRoom(room);

		room->Init();

		nlohmann::json resJson;
		resJson["result"] = "SUCCESS";
		resJson["roomId"] = room->roomId;

		resJson["ip"] = localHostIp;
		resJson["port"] = tcpPort;

		cout << resJson.dump() << endl;

		res.set_content(resJson.dump(), "application/json");
        });

	//svr.Post("/TestOfficeRoom", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {
	//	vector<string> scenes;
	//	scenes.push_back("Office");
	//	
	//	shared_ptr<OfficeRoom> room = make_shared<OfficeRoom>(scenes);

	//	//room->type = RoomType::Office;

	//	room->roomCode = "test";
	//	room->modeType = 1;
	//	room->creatorId = "test";
	//	room->currentHostId = "test";
	//	room->password = "test";
	//	room->isPassword = false;
	//	room->maxPlayerNumber = 10;
	//	room->observer = 10;
	//	room->isWaitingRoom = false;
	//	room->runningTime = 100;

	//	room->AUTO_DESTROY = false;
	//	room->DESTROY_WHEN_EMPTY = true;

	//	room->Init();

	//	GRoomManager->AddRoom(room);

	//	nlohmann::json resJson;
	//	resJson["result"] = "SUCCESS";
	//	resJson["roomId"] = room->roomId;

	//	resJson["ip"] = localHostIp;
	//	resJson["port"] = tcpPort;

	//	res.set_content(resJson.dump(), "application/json");
	//	});

//	svr.Post("/TestMyRoomRoom", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {
//		vector<string> scenes;
//		scenes.push_back("MyRoom");
//
//		std::string bodyStr;
//		content_reader([&](const char* data, size_t data_length) {
//			bodyStr.append(data, data_length);
//			return true;
//			});
//
//		nlohmann::json body = nlohmann::json::parse(bodyStr);
//
//		shared_ptr<MyRoomRoom> room = make_shared<MyRoomRoom>(scenes);
//
//		//room->type = RoomType::MyRoom;
//
//		if(body.contains("ownerId"))
//			room->ownerId = body["ownerId"];
//		else
//			room->ownerId = "test";
//
//		room->Init();
//
//		GRoomManager->AddRoom(room);
//
//		nlohmann::json resJson;
//		resJson["result"] = "SUCCESS";
//		resJson["roomId"] = room->roomId;
//
//		resJson["ip"] = localHostIp;
//		resJson["port"] = tcpPort;
//
//		res.set_content(resJson.dump(), "application/json");
//		});
//
//	svr.Post("/TestArzLandRoom", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {
//		vector<string> scenes;
//		scenes.push_back("ArzLand");
//		scenes.push_back("GameZone");
//		scenes.push_back("VoteZone");
//		scenes.push_back("CTFZone");
//
//		shared_ptr<GameRoom> room = make_shared<GameRoom>(scenes);
//
//		//room->type = RoomType::ArzLand;
//
//		room->Init();
//
//		GRoomManager->AddRoom(room);
//
//		nlohmann::json resJson;
//		resJson["result"] = "SUCCESS";
//		resJson["roomId"] = room->roomId;
//
//		resJson["ip"] = localHostIp;
//		resJson["port"] = tcpPort;
//
//		res.set_content(resJson.dump(), "application/json");
//		});
//
//    svr.Post("/Stop", [&](const httplib::Request& req, httplib::Response& res) {
//        svr.stop();
//        });

    svr.listen(ip.c_str(), port);
}

