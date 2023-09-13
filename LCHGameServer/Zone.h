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

	bool Update(milliseconds UpdateTimeStamp);

public:
	ZoneIDType zoneID;
	std::vector<ActorPtr> actorVector;
	int32 xMax;
	int32 xMin;
	int32 yMax;
	int32 yMin;
	Map zoneMap;

private:
	RecursiveMutex actorLock;
};

using ZonePtr = std::shared_ptr<Zone>;
