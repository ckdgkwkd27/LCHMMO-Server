#pragma once
class ServerSession;
using ServerSessionPtr = std::shared_ptr<ServerSession>;

class ServerPacketHandler
{
public:
	static bool HandlePacket(ServerSessionPtr session, char* buffer, uint32 len) { return true; }
};

