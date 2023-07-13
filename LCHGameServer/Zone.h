#pragma once
#include "Actor.h"
using ZoneIDType = uint32;

class Zone
{
public:
	void RegisterActor(ActorPtr _actor);
	ActorPtr FindActor(ActorIDType _actorID);
	void BroadCast(ActorPtr _selfPlayer, CircularBufferPtr _sendBuffer);

public:
	ZoneIDType zoneID;
	std::vector<ActorPtr> actorVector;
	uint32 xMax;
	uint32 yMax;
	RecursiveMutex actorLock;
};

using ZonePtr = std::shared_ptr<Zone>;
