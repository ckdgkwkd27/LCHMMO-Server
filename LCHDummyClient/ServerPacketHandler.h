#pragma once
#include "ServerSession.h"
#include "protocol.pb.h"

using ServerSessionPtr = std::shared_ptr<ServerSession>;
using ServerPacketHandlerFunc = std::function<bool(ServerSessionPtr&, char*, uint32)>;

extern ServerPacketHandlerFunc GServerPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_CS_LOGIN = 1,
	PKT_SC_LOGIN,
	PKT_CS_ENTER_GAME,
	PKT_SC_ENTER_GAME,
	PKT_CS_CHAT,
	PKT_SC_CHAT
};

bool HandleInvalid(ServerSessionPtr& session, char* buffer, uint32 len);
bool Handle_PKT_SC_LOGIN(ServerSessionPtr& session, protocol::ReturnLogin& packet);
bool Handle_PKT_SC_ENTER_GAME(ServerSessionPtr& session, protocol::ReturnEnterGame& packet);
bool Handle_PKT_SC_CHAT(ServerSessionPtr& session, protocol::ReturnChat& packet);

class ServerPacketHandler
{
public:
	static void Init();
	static bool HandlePacket(ServerSessionPtr session, char* buffer, uint32 len);
	static CircularBufferPtr MakeSendBufferPtr(protocol::RequestLogin& pkt) { return MakeSendBufferPtr(pkt, PKT_CS_LOGIN); }
	static CircularBufferPtr MakeSendBufferPtr(protocol::RequestEnterGame& pkt) { return MakeSendBufferPtr(pkt, PKT_CS_ENTER_GAME); }
	template<typename T>
	static CircularBufferPtr MakeSendBufferPtr(T& pkt, uint16 PacketID)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		CircularBufferPtr sendBuffer = std::make_shared<CircularBuffer>(MAX_BUFFER_SIZE);
		if (sendBuffer == nullptr)
			ASSERT_CRASH(false);

		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->data());
		header->size = packetSize;
		header->id = PacketID;
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		ASSERT_CRASH(sendBuffer->OnWrite(packetSize) != false);
		return sendBuffer;
	}

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, ServerSessionPtr& session, char* buffer, uint32 len);
};

template<typename PacketType, typename ProcessFunc>
inline bool ServerPacketHandler::HandlePacket(ProcessFunc func, ServerSessionPtr& session, char* buffer, uint32 len)
{
	PacketType packet;
	if (packet.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
		return false;

	return func(session, packet);
}