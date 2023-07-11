#pragma once
#include "Actor.h"
using ZoneIDType = uint32;

class Zone
{
public:
	void RegisterActor(Actor* _actor);
	Actor* FindActor(ActorIDType _actorID);
public:
	ZoneIDType zoneID;
	std::vector<Actor*> actorVector;
	uint32 xMax;
	uint32 yMax;
};

