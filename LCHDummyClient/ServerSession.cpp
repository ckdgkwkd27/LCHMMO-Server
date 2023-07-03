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

	protocol::RequestLogin loginPacket;

	uint32 totalSize = (uint32)loginPacket.ByteSizeLong() + sizeof(PacketHeader);
	if (sendBuffer.FreeSize() < totalSize)
		return;

	google::protobuf::io::ArrayOutputStream arrayOutputStream(sendBuffer.WritePos(), totalSize);
	google::protobuf::io::CodedOutputStream codedOutputStream(&arrayOutputStream);

	PacketHeader header;
	header.id = PKT_CS_LOGIN;
	header.size = (uint16)loginPacket.ByteSizeLong();

	CodedOutputStream.WriteRaw(&header, sizeof(PacketHeader));
	loginPacket.Serialize
}

void ServerSession::OnDisconnected()
{
}

void ServerSession::OnSend(uint32 len)
{
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
