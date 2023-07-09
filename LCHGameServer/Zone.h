#pragma once
#include "Actor.h"
using ZoneIDType = uint32;

class Zone
{
public:
	ZoneIDType ZoneID;
	std::unordered_map<ActorIDType, Actor*> ZoneActorMap;
};

