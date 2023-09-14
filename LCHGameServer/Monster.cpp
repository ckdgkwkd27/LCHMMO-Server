#include "pch.h"
#include "Monster.h"
#include "ZoneManager.h"
#include "RandomUtil.h"
#include "Player.h"

Monster::Monster()
{
	ActorInfo.set_objecttype((uint32)ObjectType::MONSTER);
	ActorInfo.set_actorid(GZoneManager.IssueActorID());
	ActorInfo.set_name("Monster" + std::to_string(ActorInfo.actorid()));
	ActorInfo.mutable_posinfo()->set_state((uint32)MoveState::IDLE);
	ActorInfo.mutable_posinfo()->set_posx(0);
	ActorInfo.mutable_posinfo()->set_posy(0);
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
	auto Elapsed = StateChangeTimeStamp - UpdateTimeStamp;
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

void Monster::UpdateIdle()
{
	SetMoveState(MoveState::MOVING);
}

void Monster::UpdateMoving()
{
	ZonePtr zone = GZoneManager.FindZoneByID(zoneId);
	if (zone == nullptr)
	{
		std::cout << "[FAILURE] Monster ACtorID=" << ActorInfo.actorid() << "Wrong Zone..!" << std::endl;
		delete this;
	}

	PlayerPtr player = zone->FindPlayerInCondition([])

	//Patrol
	int32 Radius = RoamRadius;
	float Angle = RandomUtil::GetRandomFloat() * 360.0f;

	int32 TargetPositionX = SpawnPosition.posx() + (int32)(Radius * cos(Angle));
	int32 TargetPositionY = SpawnPosition.posy() + (int32)(Radius * sin(Angle));

	Destination.set_posx(TargetPositionX);
	Destination.set_posy(TargetPositionY);
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
