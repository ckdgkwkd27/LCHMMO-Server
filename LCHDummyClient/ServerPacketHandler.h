#pragma once
#include "ServerSession.h"
#include "protocol.pb.h"

using ServerSessionPtr = std::shared_ptr<ServerSession>;
using ServerPacketHandlerFunc = std::function<bool(ServerSessionPtr, char*, uint32)>;

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

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandleProtobuf(ProcessFunc func, ServerSessionPtr& session, char* buffer, uint32 len);
};

template<typename PacketType, typename ProcessFunc>
inline bool ServerPacketHandler::HandleProtobuf(ProcessFunc func, ServerSessionPtr& session, char* buffer, uint32 len)
{
	PacketType packet;
	if (packet.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
		return false;

	return func(session, packet);
}
