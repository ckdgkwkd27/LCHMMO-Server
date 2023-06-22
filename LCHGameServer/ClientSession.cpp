#include "pch.h"
#include "ClientSession.h"

void ClientSession::OnConnected()
{
	GSessionManager.AddToActivePool(shared_from_this());
}

void ClientSession::OnDisconnected()
{
	GSessionManager.DeleteFromActivePool(shared_from_this());
}

void ClientSession::OnSend(uint32 len)
{
	std::cout << "OnSend() Len=" << len << std::endl;
}

uint32 ClientSession::OnRecv(char* buffer, uint32 len)
{
	uint32 processLen = 0;

	while (true)
	{
		uint32 dataSize = len - processLen;
		if(dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));
		if (dataSize < header.size)
			break;

		/*
		ClientPacketHandler::HandlePacket();
		*/

		processLen += header.size;
	}
	std::cout << "OnRecv() Len=" << len << std::endl;
	return processLen;
}
