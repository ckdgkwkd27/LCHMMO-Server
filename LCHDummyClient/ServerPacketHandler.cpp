#include "pch.h"
#include "ServerPacketHandler.h"
#include "RandomUtil.h"

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
	GServerPacketHandler[PKT_SC_SPAWN] = [](ServerSessionPtr& session, char* buffer, uint32 len)
	{
		return HandlePacket<protocol::NotifySpawn>(Handle_PKT_SC_SPAWN, session, buffer, len); 
	};
	GServerPacketHandler[PKT_SC_MOVE] = [](ServerSessionPtr& session, char* buffer, uint32 len)
	{
		return HandlePacket<protocol::ReturnMove>(Handle_PKT_SC_MOVE, session, buffer, len);
	};
	GServerPacketHandler[PKT_SC_DESPAWN] = [](ServerSessionPtr& session, char* buffer, uint32 len)
	{
		return HandlePacket<protocol::NotifyDespawn>(Handle_PKT_SC_DESPAWN, session, buffer, len);
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
	//printf("[INFO] NewPlayer=%d => ME=%d\n", packet.myplayer().actorid() , session->playerId);

	session->state = SessionState::ENTER_GAME;

	/*int32 x = RandomUtil::GetRandomRangeInt(-500, 300);
	int32 y = RandomUtil::GetRandomRangeInt(-300, 200);

	protocol::RequestTeleport teleportPkt;
	teleportPkt.set_actorid(session->playerId);
	teleportPkt.set_zoneid(1);
	teleportPkt.mutable_posinfo()->set_posx(x);
	teleportPkt.mutable_posinfo()->set_posy(x);
	auto _sendBuffer = ServerPacketHandler::MakeSendBufferPtr(teleportPkt);
	session->PostSend(_sendBuffer);
	return true;*/

	int32 x = RandomUtil::GetRandomRangeInt(-10, 10);
	int32 y = RandomUtil::GetRandomRangeInt(-10, 10);

	//RequestMove에 ActorID 넣도록?
	protocol::RequestMove movePkt;
	movePkt.mutable_posinfo()->set_posx(x);
	movePkt.mutable_posinfo()->set_posy(x);
	auto _sendBuffer = ServerPacketHandler::MakeSendBufferPtr(movePkt);
	session->PostSend(_sendBuffer);
	return true;
}

bool Handle_PKT_SC_SPAWN(ServerSessionPtr& session, protocol::NotifySpawn& packet)
{
	std::cout << "Handle_PKT_SC_SPAWN()" << std::endl;
	return true;
}

bool Handle_PKT_SC_MOVE(ServerSessionPtr& session, protocol::ReturnMove& packet)
{
	return true;
}

bool Handle_PKT_SC_DESPAWN(ServerSessionPtr& session, protocol::NotifyDespawn& packet)
{
	return true;
}