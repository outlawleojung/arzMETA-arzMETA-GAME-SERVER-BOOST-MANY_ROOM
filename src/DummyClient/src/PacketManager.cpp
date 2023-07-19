#include "PacketManager.h"
#include "Client/Client.h"
#include "Session/GameSession.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(shared_ptr<GameSession>& session, unsigned char* buffer, int len)
{
	return true;
}

bool Handle_S_ENTER(shared_ptr<GameSession>& session, Protocol::S_ENTER& pkt) 
{
	auto client = session->owner;

	GLogManager->Log("Client ", client->clientId, " S_ENTER : ", pkt.result());

	Protocol::C_BASE_INSTANTIATE_OBJECT instantiate;
	auto position = instantiate.mutable_position();
	position->set_x(client->position_x);
	position->set_y(client->position_y);
	position->set_z(client->position_z);
	
	instantiate.set_prefabname("Addressable/Prefab/Player/Player_Realtime");

	instantiate.set_objectdata("{\"1\":310007,\"4\":340036}");

	client->Send(PacketManager::MakeSendBuffer(instantiate));

	return true;
}

bool Handle_S_REENTER(shared_ptr<GameSession>& session, Protocol::S_REENTER& pkt) 
{
	return true;
}

bool Handle_S_ADD_CLIENT(shared_ptr<GameSession>& session, Protocol::S_ADD_CLIENT& pkt) 
{
	auto client = session->owner;

	string log = "";

	log.append("Client ").append(client->clientId).append(" S_ADD_CLIENT : ");

	for (int i = 0; i < pkt.clientinfos_size(); i++)
	{
		auto clientInfo = pkt.clientinfos(i);
		log.append(clientInfo.clientid()).append(" ");
	}

	GLogManager->Log(log);

	return true;
}

bool Handle_S_REMOVE_CLIENT(shared_ptr<GameSession>& session, Protocol::S_REMOVE_CLIENT& pkt) 
{
	auto client = session->owner;

	string log = "";

	log.append("Client ").append(client->clientId).append(" S_REMOVE_CLIENT : ");

	for (int i = 0; i < pkt.clientids_size(); i++)
		log.append(pkt.clientids(i)).append(" ");

	GLogManager->Log(log);

	return true;
}

bool Handle_S_SET_NICKNAME(shared_ptr<GameSession>& session, Protocol::S_SET_NICKNAME& pkt) 
{
	return true;
}

bool Handle_S_SET_NICKNAME_NOTICE(shared_ptr<GameSession>& session, Protocol::S_SET_NICKNAME_NOTICE& pkt) 
{
	return true;
}

bool Handle_S_CHAT(shared_ptr<GameSession>& session, Protocol::S_CHAT& pkt)
{
	return true;
}

bool Handle_S_DISCONNECT(shared_ptr<GameSession>& session, Protocol::S_DISCONNECT& pkt) 
{
	auto client = session->owner;

	GLogManager->Log("Client ", client->clientId, " S_DISCONNECTED : ", pkt.code());

	return true;
}

bool Handle_S_WILDCARD(shared_ptr<GameSession>& session, Protocol::S_WILDCARD& pkt)
{
	return true;
}

bool Handle_S_WILDCARD_MAP(shared_ptr<GameSession>& session, Protocol::S_WILDCARD_MAP& pkt)
{
	return true;
}

bool Handle_S_BASE_INSTANTIATE_OBJECT(shared_ptr<GameSession>& session, Protocol::S_BASE_INSTANTIATE_OBJECT& pkt) 
{
	auto client = session->owner;

	GLogManager->Log("Client ", client->clientId, " S_INSTANTIATE_OBJECT : ", to_string(pkt.objectid()));

	client->objectId = pkt.objectid();

	client->DoAsync(&Client::StartMove);

	return true;
}

bool Handle_S_BASE_ADD_OBJECT(shared_ptr<GameSession>& session, Protocol::S_BASE_ADD_OBJECT& pkt) 
{
	auto client = session->owner;

	string log = "";

	log.append("Client ").append(client->clientId).append(" S_ADD_OBJECT : ");

	for (int i = 0; i < pkt.gameobjects_size(); i++)
	{
		auto gameObjectInfo = pkt.gameobjects(i);
		log.append("\n").append("	").append(to_string(gameObjectInfo.objectid()));
		log.append("\n").append("	")
			.append(to_string(gameObjectInfo.position().x())).append(" ")
			.append(to_string(gameObjectInfo.position().y())).append(" ")
			.append(to_string(gameObjectInfo.position().z())).append(" ");
	}

	GLogManager->Log(log);

	return true;
}

bool Handle_S_BASE_REMOVE_OBJECT(shared_ptr<GameSession>& session, Protocol::S_BASE_REMOVE_OBJECT& pkt) 
{
	auto client = session->owner;

	string log = "";

	log.append("Client ").append(client->clientId).append(" S_REMOVE_OBJECT : ");
	for (int i = 0; i < pkt.gameobjects_size(); i++)
		log.append(to_string(pkt.gameobjects(i))).append(" ");

	GLogManager->Log(log);

	return true;
}

bool Handle_S_BASE_SET_OBJECT_DATA(shared_ptr<GameSession>& session, Protocol::S_BASE_SET_OBJECT_DATA& pkt) 
{
	return true;
}

bool Handle_S_BASE_SET_OBJECT_DATA_NOTICE(shared_ptr<GameSession>& session, Protocol::S_BASE_SET_OBJECT_DATA_NOTICE& pkt) 
{
	return true;
}

bool Handle_S_BASE_SET_TRANSFORM(shared_ptr<GameSession>& session, Protocol::S_BASE_SET_TRANSFORM& pkt) 
{
	//auto client = session->owner;

	//GLogManager->Log("Client ", client->clientId, " BASE_SET_TRANSFORM : ",
	//	to_string(pkt.objectid()), " ",
	//	to_string(pkt.position().x()), " ",
	//	to_string(pkt.position().y()), " ",
	//	to_string(pkt.position().z()), " "
	//);

	return true;
}

bool Handle_S_BASE_SET_ANIMATION(shared_ptr<GameSession>& session, Protocol::S_BASE_SET_ANIMATION& pkt) 
{
	return true;
}

bool Handle_S_BASE_SET_ANIMATION_ONCE(shared_ptr<GameSession>& session, Protocol::S_BASE_SET_ANIMATION_ONCE& pkt) 
{
	return true;
}

bool Handle_S_INTERACTION_GET_ITEMS(shared_ptr<GameSession>& session, Protocol::S_INTERACTION_GET_ITEMS& pkt) 
{
	return true;
}

bool Handle_S_INTERACTION_SET_ITEM(shared_ptr<GameSession>& session, Protocol::S_INTERACTION_SET_ITEM& pkt) 
{
	return true;
}

bool Handle_S_INTERACTION_SET_ITEM_NOTICE(shared_ptr<GameSession>& session, Protocol::S_INTERACTION_SET_ITEM_NOTICE& pkt) 
{
	return true;
}

bool Handle_S_INTERACTION_REMOVE_ITEM(shared_ptr<GameSession>& session, Protocol::S_INTERACTION_REMOVE_ITEM& pkt) 
{
	return true;
}

bool Handle_S_INTERACTION_REMOVE_ITEM_NOTICE(shared_ptr<GameSession>& session, Protocol::S_INTERACTION_REMOVE_ITEM_NOTICE& pkt) 
{
	return true;
}

bool Handle_S_MYROOM_GET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::S_MYROOM_GET_ROOMINFO& pkt) 
{
	return true;
}

bool Handle_S_MYROOM_SET_ROOMINFO(shared_ptr<GameSession>& session, Protocol::S_MYROOM_SET_ROOMINFO& pkt) 
{
	return true;
}

bool Handle_S_MYROOM_OTHER_ROOM_LIST(shared_ptr<GameSession>& session, Protocol::S_MYROOM_OTHER_ROOM_LIST& pkt) 
{
	return true;
}

bool Handle_S_MYROOM_START_EDIT(shared_ptr<GameSession>& session, Protocol::S_MYROOM_START_EDIT& pkt)
{
	return true;
}

bool Handle_S_MYROOM_END_EDIT(shared_ptr<GameSession>& session, Protocol::S_MYROOM_END_EDIT& pkt)
{
	return true;
}

bool Handle_S_MYROOM_KICK(shared_ptr<GameSession>& session, Protocol::S_MYROOM_KICK& pkt)
{
	return true;
}

bool Handle_S_MYROOM_SHUTDOWN(shared_ptr<GameSession>& session, Protocol::S_MYROOM_SHUTDOWN& pkt)
{
	return true;
}

bool Handle_S_OFFICE_ADD_WAITING_CLIENT(shared_ptr<GameSession>& session, Protocol::S_OFFICE_ADD_WAITING_CLIENT& pkt) 
{
	return true;
}

bool Handle_S_OFFICE_REMOVE_WAITING_CLIENT(shared_ptr<GameSession>& session, Protocol::S_OFFICE_REMOVE_WAITING_CLIENT& pkt) 
{
	return true;
}

bool Handle_S_OFFICE_ACCEPT_WAIT(shared_ptr<GameSession>& session, Protocol::S_OFFICE_ACCEPT_WAIT& pkt) 
{
	return true;
}

bool Handle_S_OFFICE_ACCEPT_WAIT_NOTICE(shared_ptr<GameSession>& session, Protocol::S_OFFICE_ACCEPT_WAIT_NOTICE& pkt) 
{
	return true;
}

bool Handle_S_OFFICE_GET_HOST(shared_ptr<GameSession>& session, Protocol::S_OFFICE_GET_HOST& pkt) 
{
	return true;
}

bool Handle_S_OFFICE_BREAK(shared_ptr<GameSession>& session, Protocol::S_OFFICE_BREAK& pkt) 
{
	return true;
}

bool Handle_S_OFFICE_KICK(shared_ptr<GameSession>& session, Protocol::S_OFFICE_KICK& pkt) 
{
	return true;
}

bool Handle_S_OFFICE_GET_PERMISSION(shared_ptr<GameSession>& session, Protocol::S_OFFICE_GET_PERMISSION& pkt) 
{
	return true;
}

bool Handle_S_OFFICE_GET_PERMISSION_ALL(shared_ptr<GameSession>& session, Protocol::S_OFFICE_GET_PERMISSION_ALL& pkt)
{
	return true;
}

bool Handle_S_OFFICE_SET_PERMISSION(shared_ptr<GameSession>& session, Protocol::S_OFFICE_SET_PERMISSION& pkt) 
{
	return true;
}

bool Handle_S_OFFICE_SET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::S_OFFICE_SET_ROOM_INFO& pkt) 
{
	return true;
}

bool Handle_S_OFFICE_GET_ROOM_INFO(shared_ptr<GameSession>& session, Protocol::S_OFFICE_GET_ROOM_INFO& pkt) 
{
	return true;
}

bool Handle_S_OFFICE_VIDEO_STREAM(shared_ptr<GameSession>& session, Protocol::S_OFFICE_VIDEO_STREAM& pkt) 
{
	return true;
}

bool Handle_S_OFFICE_SHARE(shared_ptr<GameSession>& session, Protocol::S_OFFICE_SHARE& pkt) 
{
	return true;
}

bool Handle_S_MATCHING_START(shared_ptr<GameSession>& session, Protocol::S_MATCHING_START& pkt) 
{
	return true;
}

bool Handle_S_MATCHING_AWARD(shared_ptr<GameSession>& session, Protocol::S_MATCHING_AWARD& pkt)
{
	return true;
}

bool Handle_S_MATCHING_FINISH(shared_ptr<GameSession>& session, Protocol::S_MATCHING_FINISH& pkt) 
{
	return true;
}

bool Handle_S_MATCHING_HOST(shared_ptr<GameSession>& session, Protocol::S_MATCHING_HOST& pkt) 
{
	return true;
}

bool Handle_S_MATCHING_ROUND_START(shared_ptr<GameSession>& session, Protocol::S_MATCHING_ROUND_START& pkt) 
{
	return true;
}

bool Handle_S_MATCHING_ROUND_FINISH(shared_ptr<GameSession>& session, Protocol::S_MATCHING_ROUND_FINISH& pkt) 
{
	return true;
}

bool Handle_S_MATCHING_TILES(shared_ptr<GameSession>& session, Protocol::S_MATCHING_TILES& pkt) 
{
	return true;
}

bool Handle_S_MATCHING_HINT(shared_ptr<GameSession>& session, Protocol::S_MATCHING_HINT& pkt) 
{
	return true;
}

bool Handle_S_MATCHING_PROBLEM(shared_ptr<GameSession>& session, Protocol::S_MATCHING_PROBLEM& pkt) 
{
	return true;
}

bool Handle_S_MATCHING_DESTROY(shared_ptr<GameSession>& session, Protocol::S_MATCHING_DESTROY& pkt) 
{
	return true;
}

bool Handle_S_MATCHING_QUIZ_DISAPPEAR(shared_ptr<GameSession>& session, Protocol::S_MATCHING_QUIZ_DISAPPEAR& pkt) 
{
	return true;
}

bool Handle_S_OX_START(shared_ptr<GameSession>& session, Protocol::S_OX_START& pkt) 
{
	return true;
}

bool Handle_S_OX_FINISH(shared_ptr<GameSession>& session, Protocol::S_OX_FINISH& pkt) 
{
	return true;
}

bool Handle_S_OX_HOST(shared_ptr<GameSession>& session, Protocol::S_OX_HOST& pkt) 
{
	return true;
}

bool Handle_S_OX_ROUND_START(shared_ptr<GameSession>& session, Protocol::S_OX_ROUND_START& pkt) 
{
	return true;
}

bool Handle_S_OX_ROUND_FINISH(shared_ptr<GameSession>& session, Protocol::S_OX_ROUND_FINISH& pkt) 
{
	return true;
}

bool Handle_S_OX_QUIZ(shared_ptr<GameSession>& session, Protocol::S_OX_QUIZ& pkt) 
{
	return true;
}

bool Handle_S_OX_DESTROY(shared_ptr<GameSession>& session, Protocol::S_OX_DESTROY& pkt) 
{
	return true;
}

bool Handle_S_OX_AWARD(shared_ptr<GameSession>& session, Protocol::S_OX_AWARD& pkt)
{
	return true;
}
