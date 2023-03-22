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

    svr.Post("/MakeRoom", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {

        std::string bodyStr;
        content_reader([&](const char* data, size_t data_length) {
            bodyStr.append(data, data_length);
            return true;
            });

        nlohmann::json body = nlohmann::json::parse(bodyStr);

		cout << body.dump() << endl;

		vector<string> scenes;
		auto scenesJson = body["scenes"];
		if (!scenesJson.is_null())
			for (int j = 0; j < scenesJson.size(); j++)
				scenes.push_back(scenesJson[j]);

		shared_ptr<RoomBase> room;

		if (body["type"] == "office")
		{
			room = make_shared<OfficeRoom>(scenes);

			room->type = RoomType::Office;

			static_pointer_cast<OfficeRoom>(room)->roomName = body["roomName"];
			static_pointer_cast<OfficeRoom>(room)->description = body["description"];
			static_pointer_cast<OfficeRoom>(room)->spaceInfoId = body["spaceInfoId"];
			static_pointer_cast<OfficeRoom>(room)->thumbnail = body["thumbnail"];
			static_pointer_cast<OfficeRoom>(room)->modeType = body["modeType"];			
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
		else
		{
			nlohmann::json resJson;
			resJson["result"] = "WRONG_ROOM_TYPE";
			
			res.set_content(resJson.dump(), "application/json");
			return;
		}

		room->Init();

		GRoomManager->AddRoom(room);

		nlohmann::json resJson;
		resJson["result"] = "SUCCESS";
		resJson["roomId"] = room->roomId;

		res.set_content(resJson.dump(), "application/json");
        });

	svr.Post("/TestOfficeRoom", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {
		vector<string> scenes;
		scenes.push_back("test");
		
		shared_ptr<OfficeRoom> room = make_shared<OfficeRoom>(scenes);

		room->type = RoomType::Office;

		room->modeType = 1;
		room->creatorId = "test";
		room->currentHostId = "test";
		room->password = "test";
		room->isPassword = false;
		room->maxPlayerNumber = 10;
		room->observer = 10;
		room->isWaitingRoom = false;
		room->runningTime = 100;

		room->AUTO_DESTROY = true;
		room->DESTROY_WHEN_EMPTY = true;

		room->Init();

		GRoomManager->AddRoom(room);

		nlohmann::json resJson;
		resJson["result"] = "SUCCESS";
		resJson["roomId"] = room->roomId;

		res.set_content(resJson.dump(), "application/json");
		});

    svr.Post("/Stop", [&](const httplib::Request& req, httplib::Response& res) {
        svr.stop();
        });

    svr.listen(ip.c_str(), port);
}

