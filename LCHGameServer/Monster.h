#pragma once
#include "Actor.h"

class Monster : public Actor
{
public:
	Monster();
	virtual void Update(milliseconds UpdateTimeStamp);

	void UpdateIdle();
	void UpdateMoving();
	void UpdateSkill();
	void UpdateDead();

private:
	milliseconds StateChangeTimeStamp;
	protocol::PositionInfo SpawnPosition;
	protocol::PositionInfo Destination;
	
	uint32 RoamRadius;
};

using MonsterPtr = std::shared_ptr<Monster>;