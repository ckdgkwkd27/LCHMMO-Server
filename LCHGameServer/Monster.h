#pragma once
#include "Actor.h"
#include "Zone.h"

#define STATE_CHANGE_INTERVAL 10
#define SEARCH_INTERVAL 10

class Monster : public Actor
{
public:
	Monster();
	virtual void Update(milliseconds UpdateTimeStamp);

	void UpdateIdle();
	void UpdateMoving();
	void UpdateSkill();
	void UpdateDead();
	void SetMoveState(MoveState _state);

private:
	milliseconds StateChangeTimeStamp;
	milliseconds SearchTimeStamp;

	protocol::PositionInfo SpawnPosition;
	protocol::PositionInfo Destination;
	
	ZoneIDType zoneId;
	uint32 RoamRadius;
};

using MonsterPtr = std::shared_ptr<Monster>;