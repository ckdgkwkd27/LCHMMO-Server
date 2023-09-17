#include "pch.h"
#include "Monster.h"
#include "ZoneManager.h"
#include "RandomUtil.h"
#include "Player.h"
#include "ClientPacketHandler.h"

Monster::Monster()
{
	ActorInfo.set_objecttype((uint32)ObjectType::MONSTER);
	ActorInfo.set_actorid(GZoneManager.IssueActorID());
	ActorInfo.set_name("Monster" + std::to_string(ActorInfo.actorid()));
	ActorInfo.mutable_posinfo()->set_state((uint32)MoveState::IDLE);
	ActorInfo.mutable_statinfo()->set_level(1);
	ActorInfo.mutable_statinfo()->set_hp(10);
	ActorInfo.mutable_statinfo()->set_maxhp(10);
	ActorInfo.mutable_statinfo()->set_attack(5);
	ActorInfo.mutable_statinfo()->set_speed(10);
	ActorInfo.mutable_statinfo()->set_totalexp(0);

	StateChangeTimeStamp = CURRENT_TIMESTAMP();
	RoamRadius = 10;
}

void Monster::Update(milliseconds UpdateTimeStamp)
{
	auto Elapsed = UpdateTimeStamp - StateChangeTimeStamp;

	if (Elapsed.count() > 100)
	{
		std::cout << "Elapsed=" << Elapsed.count() << std::endl;

		switch ((MoveState)ActorInfo.posinfo().state())
		{
		case MoveState::IDLE:
			UpdateIdle();
			break;
		case MoveState::MOVING:
			UpdateMoving();
			break;
		case MoveState::SKILL:
			UpdateSkill();
			break;
		case MoveState::DEAD:
			UpdateDead();
			break;
		}

		StateChangeTimeStamp = UpdateTimeStamp;
	}
}

void Monster::UpdateIdle()
{
	SetMoveState(MoveState::MOVING);
}

void Monster::UpdateMoving()
{
	ZonePtr zone = GZoneManager.FindZoneByID(zoneId);
	if (zone == nullptr)
	{
		std::cout << "[FAILURE] Monster ActorID=" << ActorInfo.actorid() << " Wrong ZoneID=" << zoneID << std::endl;
		delete this;
	}

	PlayerPtr player = zone->FindPlayerInCondition([this](ActorPtr actor)
		{
			Vector2Int dir(ActorInfo.posinfo().posx() - actor->ActorInfo.posinfo().posx(),
				ActorInfo.posinfo().posy() - actor->ActorInfo.posinfo().posy());
			return dir.CellDistFromZero <= SEARCH_CELL_DISTANCE;
		});

	if (player != nullptr)
	{
		//#TODO 공격으로 상태전환
		std::cout << "Monster Found Player=" << player->ActorInfo.name() << std::endl;
		SetMoveState(MoveState::SKILL); //#TODO Chase로 가는거 고려
	}

	//Patrol
	int32 Radius = RoamRadius;
	float Angle = RandomUtil::GetRandomFloat() * 360.0f;

	int32 TargetPositionX = SpawnPosition.posx() + (int32)(Radius * cos(Angle));
	int32 TargetPositionY = SpawnPosition.posy() + (int32)(Radius * sin(Angle));

	Destination.set_posx(TargetPositionX);
	Destination.set_posy(TargetPositionY);
	zone->zoneMap.ApplyMove(shared_from_this(), Vector2Int(TargetPositionX, TargetPositionY));

	protocol::ReturnMove movePacket;
	movePacket.set_actorid(ActorInfo.actorid());
	movePacket.mutable_posinfo()->CopyFrom(ActorInfo.posinfo());
	auto _sendBuffer = ClientPacketHandler::MakeSendBufferPtr(movePacket);
	zone->BroadCast(shared_from_this(), _sendBuffer);
}

void Monster::UpdateSkill()
{

}

void Monster::UpdateDead()
{

}

void Monster::SetMoveState(MoveState _state)
{
	ActorInfo.mutable_posinfo()->set_state((uint32)_state);
}
