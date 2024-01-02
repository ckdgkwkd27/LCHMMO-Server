#include "pch.h"
#include "ClientPacketHandler.h"
#include "PlayerManager.h"
#include "ZoneManager.h"
#include "TimeUtil.h"
#include "Viewport.h"

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
    GClientPacketHandler[PKT_CS_MOVE] = [](ClientSessionPtr& session, char* buffer, uint32 len)
	{
		return HandlePacket<protocol::RequestMove>(Handle_PKT_CS_MOVE, session, buffer, len);
	};
	GClientPacketHandler[PKT_CS_SKILL] = [](ClientSessionPtr& session, char* buffer, uint32 len)
	{
		return HandlePacket<protocol::RequestSkill>(Handle_PKT_CS_SKILL, session, buffer, len);
	};
	GClientPacketHandler[PKT_CS_TELEPORT] = [](ClientSessionPtr& session, char* buffer, uint32 len)
	{
		return HandlePacket<protocol::RequestTeleport>(Handle_PKT_CS_TELEPORT, session, buffer, len);
	};
	GClientPacketHandler[PKT_S_VIEWPORT_UDPATE] = [](ClientSessionPtr& session, char* buffer, uint32 len)
	{
		return HandlePacket<protocol::RequestViewportUpdate>(Handle_PKT_S_VIEWPORT_UPDATE, session, buffer, len);
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
    //#TODO: DB Login Check하고 Pass, Fail 통보

    auto _player = GPlayerManager.NewPlayer();
    _player->ActorInfo.set_objecttype((uint32)ObjectType::PLAYER);
    _player->ActorInfo.set_actorid(GZoneManager.IssueActorID());
    _player->ActorInfo.set_name("Player" + std::to_string(_player->playerId));
    _player->ActorInfo.mutable_posinfo()->set_state((uint32)MoveState::IDLE);
    _player->ActorInfo.mutable_posinfo()->set_movedir((uint32)MoveDir::UP);
    _player->ActorInfo.mutable_posinfo()->set_posx(0);
    _player->ActorInfo.mutable_posinfo()->set_posy(0);
    _player->ActorInfo.mutable_statinfo()->set_level(1);
    _player->ActorInfo.mutable_statinfo()->set_hp(100);
    _player->ActorInfo.mutable_statinfo()->set_maxhp(100);
    _player->ActorInfo.mutable_statinfo()->set_attack(5);
    _player->ActorInfo.mutable_statinfo()->set_speed(5);
    _player->ActorInfo.mutable_statinfo()->set_totalexp(0);
    _player->zoneID = 0;

    _player->ownerSession = session;
    session->currentPlayer = _player;

    protocol::ReturnLogin ReturnPkt;
    ReturnPkt.set_playerid(_player->playerId);
    ReturnPkt.set_success(true);
    auto _sendBuffer = ClientPacketHandler::MakeSendBufferPtr(ReturnPkt);
    session->PostSend(_sendBuffer);

    std::cout << "[INFO] Handle Login! Socket=" << session->GetSocket() << ", ID=" << packet.id() << ", Password=" << packet.password() << std::endl;
    return true;
}

bool Handle_PKT_CS_ENTER_GAME(ClientSessionPtr& session, protocol::RequestEnterGame& packet)
{
    if (session->currentPlayer->playerId != packet.playerid())
        return false;

	PlayerPtr _player = session->currentPlayer;
	RETURN_FALSE_ON_FAIL(_player != nullptr);

	ZonePtr _zone = GZoneManager.FindZoneByID(packet.zoneid());
	RETURN_FALSE_ON_FAIL(_zone != nullptr);

	_zone->messageQueue.Push([_zone, _player]() {_zone->EnterGame(_player, _zone->zoneID); });
    _player->viewport = std::make_shared<Viewport>(_player);
    _player->zoneID = _zone->zoneID;

	std::cout << "[INFO] ReturnEnterGame Packet Send Socket=" << session->GetSocket() << ", PlayerID=" << packet.playerid() << ", ZoneID=" << _zone->zoneID << std::endl;
	return true;
}

bool Handle_PKT_CS_MOVE(ClientSessionPtr& session, protocol::RequestMove& packet)
{
    PlayerPtr _player = session->currentPlayer;
    RETURN_FALSE_ON_FAIL(_player != nullptr);

    ZonePtr _zone = GZoneManager.FindZoneByID(_player->zoneID);
    RETURN_FALSE_ON_FAIL(_zone != nullptr);

    _zone->messageQueue.Push([_zone, _player, packet]() {_zone->HandleMove(_player, packet); });

    //auto now = std::chrono::high_resolution_clock::now();
    //std::cout << "이동처리 완료=" << TimeUtil::GetCurrentTime() << std::endl;
    //std::cout << "[INFO] ReturnMove Packet Send Socket=" << session->GetSocket() << ", ActorID=" << MovePkt.actorid() << std::endl;
    return true;
}

bool Handle_PKT_CS_SKILL(ClientSessionPtr& session, protocol::RequestSkill& packet)
{
    PlayerPtr _player = session->currentPlayer;
    RETURN_FALSE_ON_FAIL(_player != nullptr);

    ZonePtr _zone = GZoneManager.FindZoneByID(_player->zoneID);
    RETURN_FALSE_ON_FAIL(_zone != nullptr);

    _zone->messageQueue.Push([_zone, _player, packet] {_zone->HandleSkill(_player, packet); });
    return true;
}

bool Handle_PKT_CS_TELEPORT(ClientSessionPtr& session, protocol::RequestTeleport& packet)
{
    PlayerPtr _player = session->currentPlayer;
    RETURN_FALSE_ON_FAIL(_player != nullptr);

    ZonePtr currentZone = GZoneManager.FindZoneByID(_player->zoneID);
    RETURN_FALSE_ON_FAIL(currentZone != nullptr);

    ZonePtr newZone = GZoneManager.FindZoneByID(packet.zoneid());
    RETURN_FALSE_ON_FAIL(newZone != nullptr);

    //Player Despawn 처리, 신규 Zone 입장처리
    //currentZone->UnregisterActor(_player);

    _player->zoneID = packet.zoneid();
    _player->ActorInfo.mutable_posinfo()->set_posx(packet.posinfo().posx());
    _player->ActorInfo.mutable_posinfo()->set_posy(packet.posinfo().posy());

	//Despawn Old
    currentZone->LeaveGame(_player->ActorInfo.actorid());

    //Notify
    newZone->messageQueue.Push([newZone, _player]() {newZone->EnterGame(_player, newZone->zoneID); });

    std::cout << "[INFO] Handle TELEPORT ActorId=" << packet.actorid() << ", ZoneId=" << packet.zoneid() << std::endl;
    return true;
}

bool Handle_PKT_S_VIEWPORT_UPDATE(ClientSessionPtr& session, protocol::RequestViewportUpdate& packet)
{
	PlayerPtr _player = session->currentPlayer;
	RETURN_FALSE_ON_FAIL(_player != nullptr);
    RETURN_FALSE_ON_FAIL(_player->playerId == packet.playerid());
    
	ZonePtr _zone = GZoneManager.FindZoneByID(_player->zoneID);
	RETURN_FALSE_ON_FAIL(_zone != nullptr);

	_zone->messageQueue.Push([_player] {_player->viewport->Update(); });
    return true;
}
