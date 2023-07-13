#include "pch.h"
#include "ClientPacketHandler.h"
#include "PlayerManager.h"
#include "ZoneManager.h"

ClientPacketHandlerFunc GClientPacketHandler[UINT16_MAX];

void ClientPacketHandler::Init()
{
    for (uint32 i = 0; i < UINT16_MAX; i++)
    {
        GClientPacketHandler[i] = HandleInvalid;
    }

    GClientPacketHandler[PKT_CS_LOGIN] = [](ClientSessionPtr& session, char* buffer, uint32 len) 
    { 
        return HandlePacket<protocol::RequestLogin>(Handle_PKT_CS_LOGIN, session, buffer, len); 
    };
    GClientPacketHandler[PKT_CS_ENTER_GAME] = [](ClientSessionPtr& session, char* buffer, uint32 len)
    {
        return HandlePacket<protocol::RequestEnterGame>(Handle_PKT_CS_ENTER_GAME, session, buffer, len);
    };
}

bool ClientPacketHandler::HandlePacket(ClientSessionPtr session, char* buffer, uint32 len)
{
    PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
    return GClientPacketHandler[header->id](session, buffer, len);
}

bool HandleInvalid(ClientSessionPtr& session, char* buffer, uint32 len)
{
    PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
    std::cout << "[FAIL] Invalid Packet. Socket=" << session->GetSocket() << ", ID=" << header->id << ", len = " << len << std::endl;
    return false;
}

bool Handle_PKT_CS_LOGIN(ClientSessionPtr& session, protocol::RequestLogin& packet)
{
    std::cout << "[INFO] Handle Login! Socket= " << session->GetSocket() << ", ID=" << packet.id() << ", Password=" << packet.password() << std::endl;
    
    //#TODO: DB Login Check하고 Pass, Fail 통보

    auto _player = GPlayerManager.NewPlayer();
    _player->actorId = GZoneManager.IssueActorID();
    _player->ownerSession = session;
    session->currentPlayer = _player;

    protocol::ReturnLogin ReturnPkt;
    ReturnPkt.set_playerid(_player->playerId);
    ReturnPkt.set_success(true);
    auto _sendBuffer = ClientPacketHandler::MakeSendBufferPtr(ReturnPkt);
    session->PostSend(_sendBuffer);
    return true;
}

bool Handle_PKT_CS_ENTER_GAME(ClientSessionPtr& session, protocol::RequestEnterGame& packet)
{
    if (session->currentPlayer->playerId != packet.playerid())
        return false;

    //Zone Spawn
    PlayerPtr player = GPlayerManager.FindPlayerByID(packet.playerid());
    if (false == GZoneManager.RegisterActor(0, player))
        return false;

    printf("{DEBUG} Player Register %d\n", player->playerId);
    //BroadCast
    auto zone = GZoneManager.FindZoneByID(0);
    if (zone == nullptr)
        return false;

    protocol::ReturnEnterGame ReturnPkt;
    ReturnPkt.set_actorid(player->actorId);
    ReturnPkt.set_playerid(player->playerId);
    auto _sendBuffer = ClientPacketHandler::MakeSendBufferPtr(ReturnPkt);
    zone->BroadCast(player, _sendBuffer);

    //#TODO
    /*
    * 0 -> 1 // 1 -> 0 과 같이 통보순서가 이상한 문제 해결해야 함
    */

    std::cout << "[INFO] ReturnEnterGame Packet Send Socket=" << session->GetSocket() << ", PlayerID=" << packet.playerid() << std::endl;
    return false;
}

bool Handle_PKT_CS_CHAT(ClientSessionPtr& session, protocol::RequestChat& packet)
{
    session->GetSocket();
    packet.msg();
    return false;
}
