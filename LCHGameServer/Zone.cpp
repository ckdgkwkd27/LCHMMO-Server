#include "pch.h"
#include "Zone.h"
#include "ClientSession.h"
#include "ZoneManager.h"
#include "PlayerManager.h"

void Zone::Init()
{
	zoneMap.LoadMap(zoneID);
	this->xMax = zoneMap.MaxX;
	this->xMin = zoneMap.MinX;
	this->yMax = zoneMap.MaxY;
	this->yMin = zoneMap.MinY;
}

void Zone::RegisterActor(ActorPtr _actor)
{
	//RecursiveLockGuard guard(actorLock);

	if(_actor == nullptr)
		CRASH_ASSERT(false);
	actorVector.push_back(_actor);
}

ActorPtr Zone::FindActor(ActorIDType _actorID)
{
	//RecursiveLockGuard guard(actorLock);

	auto it = std::find_if(actorVector.begin(), actorVector.end(), [_actorID](ActorPtr _actor)
		{
			if (_actor == nullptr) CRASH_ASSERT(false);
			return _actor->ActorInfo.actorid() == _actorID;
		});
	if (it == actorVector.end())
		return nullptr;

	return *it;
}

PlayerPtr Zone::FindPlayerInCondition(std::function<bool(ActorPtr)> _condition)
{
	//RecursiveLockGuard guard(actorLock);

	for (ActorPtr _actor : actorVector)
	{
		if (_actor->ActorInfo.objecttype() == (uint32)ObjectType::PLAYER)
		{
			if (_condition(_actor) == true)
			{
				return std::dynamic_pointer_cast<Player>(_actor);
			}
		}
	}
	return nullptr;
}

void Zone::BroadCast(ActorPtr _selfPlayer, CircularBufferPtr _sendBuffer)
{
	//RecursiveLockGuard guard(actorLock);

	for (auto& _actor : actorVector)
	{
		PlayerPtr _player = nullptr;
		_player = std::dynamic_pointer_cast<Player>(_actor);
		if (_player != nullptr && _player != _selfPlayer)
		{
			//printf("ME=%lld => OTHER=%lld\n", _selfPlayer->ActorInfo.actorid(), _player->ActorInfo.actorid());
			//printf("_player SOCKET=%lld", _player->ownerSession->GetSocket());
			_player->ownerSession->PostSend(_sendBuffer);
		}
	}
}

bool Zone::Update()
{
	//RecursiveLockGuard guard(actorLock);

	//Actor Update
	for (ActorPtr _actor : actorVector)
	{
		_actor->Update();
	}

	//Message Queue Process
	std::vector<MessageFuncType> messageVector;
	messageQueue.FlushAll(messageVector);

	for (MessageFuncType message : messageVector)
	{
		message();
	}

	return true;
}

void Zone::EnterGame(PlayerPtr player, protocol::RequestEnterGame enterPacket)
{
	auto zone = GZoneManager.FindZoneByID(0);
	if (zone == nullptr)
		return;

	if (false == GZoneManager.RegisterActor(0, player))
		return;

	zone->zoneMap.ApplyMove(player, Vector2Int(player->ActorInfo.posinfo().posx(), player->ActorInfo.posinfo().posy()));

	//Player 게임입장
	{
		protocol::ReturnEnterGame ReturnPkt;
		ReturnPkt.mutable_myplayer()->CopyFrom(player->ActorInfo);
		ReturnPkt.set_zoneid(player->zoneID);
		auto _sendBuffer = ClientPacketHandler::MakeSendBufferPtr(ReturnPkt);
		player->ownerSession->PostSend(_sendBuffer);
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

void Zone::LeaveGame(ActorIDType _actorId)
{
	auto actor = FindActor(_actorId);
	if (actor->ActorInfo.objecttype() == (uint32)ObjectType::PLAYER)
	{
		PlayerPtr player = std::dynamic_pointer_cast<Player>(actor);
		GPlayerManager.DeletePlayer(player);

		zoneMap.ApplyLeave(actor);

		protocol::NotifyDespawn despawnPacket;
		auto sendBuffer = ClientPacketHandler::MakeSendBufferPtr(despawnPacket);
		player->ownerSession->PostSend(sendBuffer);
	}

	//BroadCast
	{
		protocol::NotifyDespawn despawnPacket;
		despawnPacket.add_actorids(_actorId);
		auto sendBuffer = ClientPacketHandler::MakeSendBufferPtr(despawnPacket);
		BroadCast(nullptr, sendBuffer);
	}
}

void Zone::HandleMove(PlayerPtr player, protocol::RequestMove movePacket)
{
	if (player == nullptr)
		return;

	//Verify
	protocol::PositionInfo movePosInfo = movePacket.posinfo();
	if (movePosInfo.posx() != player->ActorInfo.posinfo().posx() || movePosInfo.posy() != player->ActorInfo.posinfo().posy())
	{
		if (zoneMap.CanGo(Vector2Int(movePosInfo.posx(), movePosInfo.posy())) == false)
			return;
	}

	player->ActorInfo.mutable_posinfo()->set_state(movePosInfo.state());
	player->ActorInfo.mutable_posinfo()->set_movedir(movePosInfo.movedir());
	zoneMap.ApplyMove(player, Vector2Int(movePosInfo.posx(), movePosInfo.posy()));

	//BroadCast
	protocol::ReturnMove resMovePacket;
	resMovePacket.set_actorid(player->ActorInfo.actorid());
	resMovePacket.mutable_posinfo()->CopyFrom(movePosInfo);
	auto _sendBuffer = ClientPacketHandler::MakeSendBufferPtr(resMovePacket);
	BroadCast(player, _sendBuffer);
}

void Zone::HandleSkill(PlayerPtr player, protocol::RequestSkill packet)
{
	if (player == nullptr)
		return;

	double dist = 0.f;
	ActorPtr targetActor;

	//Check Distance
	{
		ZonePtr	_zone = GZoneManager.FindZoneByID(player->zoneID);
		auto it = std::find_if(_zone->actorVector.begin(), _zone->actorVector.end(), [packet](ActorPtr _actor)
		{
			return _actor->ActorInfo.actorid() == packet.targetactorid();
		});
		if (it == _zone->actorVector.end())
			return;

		targetActor = *it;
		Vector2Int targetPos(targetActor->ActorInfo.posinfo().posx(), targetActor->ActorInfo.posinfo().posy());
		Vector2Int playerPos(player->ActorInfo.posinfo().posx(), player->ActorInfo.posinfo().posy());
		dist = Vector2Int::Distance(targetPos, playerPos);
	}

	if (packet.skillid() == 2)
	{
		if (dist > 3.f)
			return;

		targetActor->OnDamaged(player, player->ActorInfo.statinfo().attack());
		printf("INFO: Player=%s Melee Atack to Monster=%s Damage=%d\n", player->ActorInfo.name().c_str(), targetActor->ActorInfo.name().c_str(), player->ActorInfo.statinfo().attack());
	}

	else if (packet.skillid() == 3)
	{

	}

	//BroadCast
	protocol::ReturnSkill resSkillPacket;
	resSkillPacket.set_actorid(player->ActorInfo.actorid());
	resSkillPacket.set_skillid(packet.skillid());
	auto _sendBuffer = ClientPacketHandler::MakeSendBufferPtr(resSkillPacket);
	BroadCast(nullptr, _sendBuffer);
}
