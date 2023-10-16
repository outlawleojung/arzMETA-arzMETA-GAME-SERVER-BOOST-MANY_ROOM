#include "HttpServer.h"
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "../Contents/GameContents.h"
#include "../Contents/ClientManager.h"

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

	svr.Get("/ExpositionBoothPlayerNumber", [](const httplib::Request& req, httplib::Response& res) {

		map<string, string> query;

		for (auto param = req.params.begin(); param != req.params.end(); param++)
			query[param->first] = param->second;

		nlohmann::json resJson;

		auto room = GRoomManager->ExpositionBoothRooms.find(query["boothId"]);
		if(room == GRoomManager->ExpositionBoothRooms.end())
			resJson["PlayerNumber"] = 0;
		else
			resJson["PlayerNumber"] = room->second->clients.size();

		res.set_content(resJson.dump(), "application/json");
		});

	svr.Post("/ExpositionBoothPlayerNumber", [](const httplib::Request& req, httplib::Response& res) {

		map<string, string> query;

		for (auto param = req.params.begin(); param != req.params.end(); param++)
			query[param->first] = param->second;

		nlohmann::json resJson;

		auto room = GRoomManager->ExpositionBoothRooms.find(query["roomCode"]);
		if (room == GRoomManager->ExpositionBoothRooms.end())
			resJson["PlayerNumber"] = 0;
		else
			resJson["PlayerNumber"] = room->second->clients.size();

		res.set_content(resJson.dump(), "application/json");
		});

    svr.Post("/MakeRoom", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {

        std::string bodyStr;
        content_reader([&](const char* data, size_t data_length) {
            bodyStr.append(data, data_length);
            return true;
            });

        nlohmann::json body = nlohmann::json::parse(bodyStr);
		
		std::cout << body.dump() << std::endl;

		//GLogManager->Log("MakeRoom Request : ", body.dump());

		auto type = stringToRoomType(body["roomType"].get<string>());

		shared_ptr<RoomBase> room;

		switch (type)
		{
		case RoomType::Arz :
		case RoomType::Game :
		case RoomType::Conference :
		case RoomType::Vote :
		case RoomType::Store:
		case RoomType::Office:
		case RoomType::Busan :
		case RoomType::Festival :
		case RoomType::Exposition:
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
		case RoomType::Meeting :
		{
			room = make_shared<MeetingRoom>();

			static_pointer_cast<MeetingRoom>(room)->roomCode = body["roomCode"];
			static_pointer_cast<MeetingRoom>(room)->roomName = body["roomName"];
			static_pointer_cast<MeetingRoom>(room)->description = body["description"];
			static_pointer_cast<MeetingRoom>(room)->spaceInfoId = body["spaceInfoId"];
			static_pointer_cast<MeetingRoom>(room)->thumbnail = body["thumbnail"];
			static_pointer_cast<MeetingRoom>(room)->topicType = body["topicType"];
			static_pointer_cast<MeetingRoom>(room)->creatorId = body["creatorId"];

			static_pointer_cast<MeetingRoom>(room)->password = body["password"];
			if (body["password"] == "")
				static_pointer_cast<MeetingRoom>(room)->isPassword = false;
			else
				static_pointer_cast<MeetingRoom>(room)->isPassword = true;

			static_pointer_cast<MeetingRoom>(room)->maxPlayerNumber = body["personnel"];
			static_pointer_cast<MeetingRoom>(room)->isWaitingRoom = body["isWaitingRoom"];
			static_pointer_cast<MeetingRoom>(room)->isAdvertising = body["isAdvertising"];
			static_pointer_cast<MeetingRoom>(room)->runningTime = body["runningTime"];

			break;
		}
		case RoomType::Lecture :
		{
			room = make_shared<LectureRoom>();

			static_pointer_cast<LectureRoom>(room)->roomCode = body["roomCode"];
			static_pointer_cast<LectureRoom>(room)->roomName = body["roomName"];
			static_pointer_cast<LectureRoom>(room)->description = body["description"];
			static_pointer_cast<LectureRoom>(room)->spaceInfoId = body["spaceInfoId"];
			static_pointer_cast<LectureRoom>(room)->thumbnail = body["thumbnail"];
			static_pointer_cast<LectureRoom>(room)->topicType = body["topicType"];
			static_pointer_cast<LectureRoom>(room)->creatorId = body["creatorId"];

			static_pointer_cast<LectureRoom>(room)->password = body["password"];
			if (body["password"] == "")
				static_pointer_cast<LectureRoom>(room)->isPassword = false;
			else
				static_pointer_cast<LectureRoom>(room)->isPassword = true;

			static_pointer_cast<LectureRoom>(room)->maxPlayerNumber = body["personnel"];
			static_pointer_cast<LectureRoom>(room)->maxObserverNumber = body["observer"];
			static_pointer_cast<LectureRoom>(room)->isWaitingRoom = body["isWaitingRoom"];
			static_pointer_cast<LectureRoom>(room)->isAdvertising = body["isAdvertising"];
			static_pointer_cast<LectureRoom>(room)->runningTime = body["runningTime"];

			break;
		}
		case RoomType::Consulting:
		{
			room = make_shared<MeetingRoom>();

			static_pointer_cast<MeetingRoom>(room)->roomCode = body["roomCode"];
			static_pointer_cast<MeetingRoom>(room)->roomName = body["roomName"];
			static_pointer_cast<MeetingRoom>(room)->description = body["description"];
			
			static_pointer_cast<MeetingRoom>(room)->creatorId = body["creatorId"];

			static_pointer_cast<MeetingRoom>(room)->maxPlayerNumber = body["personnel"];
			static_pointer_cast<MeetingRoom>(room)->runningTime = body["runningTime"];

			static_pointer_cast<MeetingRoom>(room)->password = "";
			static_pointer_cast<MeetingRoom>(room)->isPassword = false;

			break;
		}
		case RoomType::JumpingMatching :
		{
			room = make_shared<MatchingRoom>();

			room->maxPlayerNumber = body["maxPlayerNumber"];
			static_pointer_cast<MatchingRoom>(room)->roomName = body["roomName"];

			break;
		}
		case RoomType::OXQuiz :
		{
			room = make_shared<OXRoom>();

			room->maxPlayerNumber = body["maxPlayerNumber"];
			static_pointer_cast<OXRoom>(room)->roomName = body["roomName"];

			break;
		}
		case RoomType::Exposition_Booth :
		{
			room = make_shared<ExpositionBoothRoom>();

			static_pointer_cast<ExpositionBoothRoom>(room)->roomCode = body["roomCode"];

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
		room->sceneName = body["sceneName"];

		GRoomManager->AddRoom(room);

		room->Init();

		nlohmann::json resJson;
		resJson["result"] = "SUCCESS";
		resJson["roomId"] = room->roomId;
		resJson["sceneName"] = room->sceneName;
		resJson["roomType"] = roomTypeToString(room->type);

		res.set_content(resJson.dump(), "application/json");
        });

	svr.Post("/Login", [](const httplib::Request& req, httplib::Response& res, const httplib::ContentReader& content_reader) {

		std::string bodyStr;
		content_reader([&](const char* data, size_t data_length) {
			bodyStr.append(data, data_length);
			return true;
			});

		nlohmann::json body = nlohmann::json::parse(bodyStr);

		GLogManager->Log("Login Request : ", body.dump());

		int sessionId = GClientManager->SetSessionId(body["clientId"]);

		nlohmann::json resJson;
		resJson["sessionId"] = sessionId;

		res.set_content(resJson.dump(), "application/json");
		});



    svr.listen(ip.c_str(), port);
}

