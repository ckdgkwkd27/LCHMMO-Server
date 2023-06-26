#pragma once
class ClientSession;
using ClientSessionPtr = std::shared_ptr<ClientSession>;

class ClientPacketHandler
{
public:
	static bool HandlePacket(ClientSessionPtr session, char* buffer, uint32 len) { return true; }
};

