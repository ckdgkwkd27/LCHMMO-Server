#pragma once
#include "Actor.h"
#include "Map.h"
using ZoneIDType = uint32;

class Zone
{
public:
	void Init();
	void RegisterActor(ActorPtr _actor);
	ActorPtr FindActor(ActorIDType _actorID);
	void BroadCast(ActorPtr _selfPlayer, CircularBufferPtr _sendBuffer);

public:
	ZoneIDType zoneID;
	std::vector<ActorPtr> actorVector;
	uint32 xMax;
	uint32 yMax;
	Map zoneMap;

private:
	RecursiveMutex actorLock;
};

using ZonePtr = std::shared_ptr<Zone>;
