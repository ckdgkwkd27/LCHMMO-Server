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

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandleProtobuf(ProcessFunc func, ClientSessionPtr& session, char* buffer, uint32 len);
};

template<typename PacketType, typename ProcessFunc>
inline bool ClientPacketHandler::HandleProtobuf(ProcessFunc func, ClientSessionPtr& session, char* buffer, uint32 len)
{
	PacketType packet;
	if (packet.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
		return false;

	return func(session, packet);
}
