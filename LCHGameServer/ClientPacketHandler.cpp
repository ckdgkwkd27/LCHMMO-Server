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
    GClientPacketHandler[PKT_CS_MOVE] = [](ClientSessionPtr& session, char* buffer, uint32 len)
	{
		return HandlePacket<protocol::RequestMove>(Handle_PKT_CS_MOVE, session, buffer, len);
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
    _player->ActorInfo.mutable_posinfo()->set_posx(0);
    _player->ActorInfo.mutable_posinfo()->set_posy(0);
    _player->ActorInfo.mutable_statinfo()->set_level(1);
    _player->ActorInfo.mutable_statinfo()->set_hp(100);
    _player->ActorInfo.mutable_statinfo()->set_maxhp(100);
    _player->ActorInfo.mutable_statinfo()->set_attack(5);
    _player->ActorInfo.mutable_statinfo()->set_speed(10);
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

    {
		LockGuard guard(GZoneManager.zoneLock);

        //Zone Spawn
        PlayerPtr player = GPlayerManager.FindPlayerByID(packet.playerid());
        if (false == GZoneManager.RegisterActor(0, player))
            return false;

        //BroadCast
        auto zone = GZoneManager.FindZoneByID(0);
        if (zone == nullptr)
            return false;

        //Player 게임입장
		{
			protocol::ReturnEnterGame ReturnPkt;
            ReturnPkt.mutable_myplayer()->CopyFrom(player->ActorInfo);
			ReturnPkt.set_zoneid(player->zoneID);
			auto _sendBuffer = ClientPacketHandler::MakeSendBufferPtr(ReturnPkt);
			session->PostSend(_sendBuffer);
		}

        //Player에게 다른사람을 알린다
		{
			protocol::NotifySpawn SpawnPkt;
			for (auto otherActor : zone->actorVector)
			{
				if (otherActor->ActorInfo.actorid() != player->ActorInfo.actorid())
				{
                    protocol::ObjectInfo* playerInfo = SpawnPkt.add_objects();
                    *playerInfo = otherActor->ActorInfo;
				}
			}

            auto _sendBuffer = ClientPacketHandler::MakeSendBufferPtr(SpawnPkt);
            player->ownerSession->PostSend(_sendBuffer);
		}

        //다른 사람들에게 Player를 알린다.
        {
            protocol::NotifySpawn SpawnPkt;
            protocol::ObjectInfo* playerInfo = SpawnPkt.add_objects();
            *playerInfo = player->ActorInfo;
            auto _broadCastBuffer = ClientPacketHandler::MakeSendBufferPtr(SpawnPkt);
            zone->BroadCast(player, _broadCastBuffer);
        }
    }

    std::cout << "[INFO] ReturnEnterGame Packet Send Socket=" << session->GetSocket() << ", PlayerID=" << packet.playerid() << std::endl;
    return true;
}

bool Handle_PKT_CS_MOVE(ClientSessionPtr& session, protocol::RequestMove& packet)
{
    PlayerPtr _player = session->currentPlayer;
    RETURN_FALSE_ON_FAIL(_player != nullptr);

    ZonePtr _zone = GZoneManager.FindZoneByID(_player->zoneID);
    RETURN_FALSE_ON_FAIL(_zone != nullptr);

    _player->ActorInfo.mutable_posinfo()->CopyFrom(packet.posinfo());

    protocol::ReturnMove MovePkt;
    MovePkt.set_actorid(_player->ActorInfo.actorid());
    MovePkt.mutable_posinfo()->CopyFrom(packet.posinfo());
    auto _broadCastBuffer = ClientPacketHandler::MakeSendBufferPtr(MovePkt);
    _zone->BroadCast(_player, _broadCastBuffer);

    std::cout << "[INFO] ReturnMove Packet Send Socket=" << session->GetSocket() << ", ActorID=" << MovePkt.actorid() << std::endl;
    return true;
}

bool Handle_PKT_CS_CHAT(ClientSessionPtr& session, protocol::RequestChat& packet)
{
    session->GetSocket();
    packet.msg();
    return true;
}
