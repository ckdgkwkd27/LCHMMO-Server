#pragma once
#include "ClientSession.h"
#include "protocol.pb.h"

using ClientSessionPtr = std::shared_ptr<ClientSession>;
using ClientPacketHandlerFunc = std::function<bool(ClientSessionPtr&, char*, uint32)>;

extern ClientPacketHandlerFunc GClientPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_CS_LOGIN = 1,
	PKT_SC_LOGIN,
	PKT_CS_ENTER_GAME,
	PKT_SC_ENTER_GAME,
	PKT_SC_SPAWN,
	PKT_CS_CHAT,
	PKT_SC_CHAT
};

bool HandleInvalid(ClientSessionPtr& session, char* buffer, uint32 len);
bool Handle_PKT_CS_LOGIN(ClientSessionPtr& session, protocol::RequestLogin& packet);
bool Handle_PKT_CS_ENTER_GAME(ClientSessionPtr& session, protocol::RequestEnterGame& packet);
bool Handle_PKT_CS_CHAT(ClientSessionPtr& session, protocol::RequestChat& packet);

class ClientPacketHandler
{
public:
	static void Init();
	static bool HandlePacket(ClientSessionPtr session, char* buffer, uint32 len);
	static CircularBufferPtr MakeSendBufferPtr(protocol::ReturnLogin& pkt) { return MakeSendBufferPtr(pkt, PKT_SC_LOGIN); }
	static CircularBufferPtr MakeSendBufferPtr(protocol::ReturnEnterGame& pkt) { return MakeSendBufferPtr(pkt, PKT_SC_ENTER_GAME); }
	static CircularBufferPtr MakeSendBufferPtr(protocol::NotifySpawn& pkt) { return MakeSendBufferPtr(pkt, PKT_SC_ENTER_GAME); }

	template<typename T>
	static CircularBufferPtr MakeSendBufferPtr(T& pkt, uint16 PacketID)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		CircularBufferPtr sendBuffer = std::make_shared<CircularBuffer>(MAX_BUFFER_SIZE);
		if (sendBuffer == nullptr)
			CRASH_ASSERT(false);

		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->data());
		header->size = packetSize;
		header->id = PacketID;
		CRASH_ASSERT(pkt.SerializeToArray(&header[1], dataSize));
		CRASH_ASSERT(sendBuffer->OnWrite(packetSize) != false);
		return sendBuffer;
	}

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, ClientSessionPtr& session, char* buffer, uint32 len);
};

template<typename PacketType, typename ProcessFunc>
inline bool ClientPacketHandler::HandlePacket(ProcessFunc func, ClientSessionPtr& session, char* buffer, uint32 len)
{
	PacketType packet;
	if (packet.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
		return false;

	return func(session, packet);
}
