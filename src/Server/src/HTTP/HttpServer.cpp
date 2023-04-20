#include "HttpServer.h"
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "../Contents/GameContents.h"

void HttpServer::start(string ip, int port)
{
    httplib::Server svr;

    svr.Get("/Rooms", [](const httplib::Request& req, httplib::Response& res) {

		map<string, string> query;

		for (auto param = req.params.begin(); param != req.params.end(); param++)
			query[param->first] = param->second;

        res.set_content(GRoomManager->GetRoom(query).dump(), "application/json");
        });

	svr.Post("/Rooms", [](const httplib::Request& req, httplib::Response& res) {

		map<string, string> query;

		for (auto param = req.params.begin(); param != req.params.end(); param++)
			query[param->first] = param->second;

		res.set_content(GRoomManager->GetRoom(query).dump(), "application/json");
		});

    svr.Post("/MakeRoom", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {

        std::string bodyStr;
        content_reader([&](const char* data, size_t data_length) {
            bodyStr.append(data, data_length);
            return true;
            });

        nlohmann::json body = nlohmann::json::parse(bodyStr);
		
		auto type = stringToRoomType(body["roomType"].get<string>());

		shared_ptr<RoomBase> room;

		switch (type)
		{
		case RoomType::ArzLand :
		case RoomType::GameZone :
		case RoomType::CTFZone :
		case RoomType::VoteZone :
		case RoomType::BusanLand :
		{
			room = make_shared<GameRoom>();
			break;
		}
		case RoomType::MyRoom :
		{
			map<string, string> query;
			query.insert({ "type", "MyRoom" });
			query.insert({ "ownerId", body["ownerId"] });
			if (GRoomManager->GetRoom(query).size() > 0)
			{
				nlohmann::json resJson;
				resJson["result"] = "MYROOM_ALREADY_EXISTS";
				res.set_content(resJson.dump(), "application/json");
				return;
			}

			room = make_shared<MyRoomRoom>();

			static_pointer_cast<MyRoomRoom>(room)->ownerId = body["ownerId"];

			break;
		}
		case RoomType::Office :
		case RoomType::Meeting :
		case RoomType::Lecture :
		case RoomType::Counsel :
		{
			room = make_shared<OfficeRoom>();

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

			break;
		}
		case RoomType::JumpingMatching :
		{
			room = make_shared<MatchingRoom>();

			room->maxPlayerNumber = body["maxPlayerNumber"];
			static_pointer_cast<MatchingRoom>(room)->roomName = body["roomName"];

			break;
		}
		case RoomType::OX :
		{
			room = make_shared<OXRoom>();

			room->maxPlayerNumber = body["maxPlayerNumber"];
			static_pointer_cast<OXRoom>(room)->roomName = body["roomName"];

			break;
		}
		default :
		{
			nlohmann::json resJson;
			resJson["result"] = "WRONG_ROOM_TYPE";

			res.set_content(resJson.dump(), "application/json");
			return;
		}
		}

		room->type = type;

		GRoomManager->AddRoom(room);

		room->Init();

		nlohmann::json resJson;
		resJson["result"] = "SUCCESS";
		resJson["roomId"] = room->roomId;

		resJson["ip"] = localHostIp;
		resJson["port"] = tcpPort;

		res.set_content(resJson.dump(), "application/json");
        });

    svr.listen(ip.c_str(), port);
}

