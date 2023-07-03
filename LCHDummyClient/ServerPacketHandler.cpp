#include "pch.h"
#include "ServerPacketHandler.h"

ServerPacketHandlerFunc GServerPacketHandler[UINT16_MAX];

void ServerPacketHandler::Init()
{
}

bool ServerPacketHandler::HandlePacket(ServerSessionPtr session, char* buffer, uint32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	return GServerPacketHandler[header->id](session, buffer, len);
}

bool HandleInvalid(ServerSessionPtr& session, char* buffer, uint32 len)
{
	return false;
}

bool Handle_PKT_SC_LOGIN(ServerSessionPtr& session, protocol::ReturnLogin& packet)
{
	return false;
}

bool Handle_PKT_SC_ENTER_GAME(ServerSessionPtr& session, protocol::ReturnEnterGame& packet)
{
	return false;
}

bool Handle_PKT_SC_CHAT(ServerSessionPtr& session, protocol::ReturnChat& packet)
{
	return false;
}
