#include "pch.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"

bool GConnected = false;

void ServerSession::OnAccepted()
{
}

void ServerSession::OnConnected()
{
	std::cout << "[INFO] Server OnConnected..!" << std::endl;
	GConnected = true;

	for (int i = 0; i < 3; i++)
	{
		protocol::RequestLogin loginPacket;
		loginPacket.set_id("test_id");
		loginPacket.set_password("test_pw");
		auto _sendBuffer = ServerPacketHandler::MakeSendBufferPtr(loginPacket);

		PostSend(_sendBuffer);
		std::this_thread::sleep_for(1ms);
	}
}

void ServerSession::OnDisconnected()
{
}

void ServerSession::OnSend(uint32 len)
{
	std::cout << "[INFO] Send Bytes=" << len << std::endl;
}

uint32 ServerSession::OnRecv(char* buffer, uint32 len)
{
	uint32 processLen = 0;

	while (true)
	{
		uint32 dataSize = len - processLen;
		if(dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));
		if(dataSize < header.size)
			break;

		ServerPacketHandler::HandlePacket(std::dynamic_pointer_cast<ServerSession>(shared_from_this()), buffer, len);
		processLen += header.size;
	}

	return processLen;
}
