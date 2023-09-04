#pragma once
#include "Zone.h"
#include "ObjectPool.h"

enum ZoneIDEnum
{
	START_ZONE,

	CNT
};

class ZoneManager
{
public:
	ZoneManager();
	void Initialize();
	void RegisterZone(ZonePtr _zone);
	bool RegisterActor(ZoneIDType _zoneId, ActorPtr _actor);
	ActorIDType IssueActorID();
	ZonePtr FindZoneByID(ZoneIDType _zoneId);

public:
	std::vector<ZonePtr> zoneVector;
	std::shared_ptr<ObjectPool<Zone>> zonePool;
	uint32 numOfActors;

public:
	RecursiveMutex zoneLock;
};

extern ZoneManager GZoneManager;
