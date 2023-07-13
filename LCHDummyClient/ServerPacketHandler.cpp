#include "pch.h"
#include "ServerPacketHandler.h"

ServerPacketHandlerFunc GServerPacketHandler[UINT16_MAX];

void ServerPacketHandler::Init()
{
	for (uint32 i = 0; i < UINT16_MAX; i++)
	{
		GServerPacketHandler[i] = HandleInvalid;
	}

	GServerPacketHandler[PKT_SC_LOGIN] = [](ServerSessionPtr& session, char* buffer, uint32 len) 
	{ 
		return HandlePacket<protocol::ReturnLogin>(Handle_PKT_SC_LOGIN, session, buffer, len); 
	};
	GServerPacketHandler[PKT_SC_ENTER_GAME] = [](ServerSessionPtr& session, char* buffer, uint32 len) 
	{ 
		return HandlePacket<protocol::ReturnEnterGame>(Handle_PKT_SC_ENTER_GAME, session, buffer, len);
	};
}

bool ServerPacketHandler::HandlePacket(ServerSessionPtr session, char* buffer, uint32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	return GServerPacketHandler[header->id](session, buffer, len);
}

bool HandleInvalid(ServerSessionPtr& session, char* buffer, uint32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	std::cout << "[FAIL] Invalid Packet. Socket=" << session->GetSocket() << ", ID=" << header->id << ", len = " << len << std::endl;
	return false;
}

bool Handle_PKT_SC_LOGIN(ServerSessionPtr& session, protocol::ReturnLogin& packet)
{
	if (session->state == SessionState::NONE)
		return false;
	
	std::cout << "[INFO] Login Completed PlayerID: " << packet.playerid() << std::endl;
	session->playerId = packet.playerid();
	session->state = SessionState::LOGIN;

	protocol::RequestEnterGame RequestPkt;
	RequestPkt.set_playerid(packet.playerid());
	auto _sendBuffer = ServerPacketHandler::MakeSendBufferPtr(RequestPkt);
	session->PostSend(_sendBuffer);
	return true;
}

bool Handle_PKT_SC_ENTER_GAME(ServerSessionPtr& session, protocol::ReturnEnterGame& packet)
{
	//#TODO Spawn In Game
	printf("[INFO] ME=%d, NewPlayer=%d\n", session->playerId, packet.playerid());

	session->state = SessionState::ENTER_GAME;
	return true;
}

bool Handle_PKT_SC_CHAT(ServerSessionPtr& session, protocol::ReturnChat& packet)
{
	return false;
}

//Session State는 서버의 상태값이니까 전혀 의미가 없는거 아닐까?
//이전에 SendBuffer를 지워버려서 의미 없어지는건가?