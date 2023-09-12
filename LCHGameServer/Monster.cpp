#include "pch.h"
#include "Monster.h"
#include "ZoneManager.h"

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
}

void Monster::Update()
{
}
