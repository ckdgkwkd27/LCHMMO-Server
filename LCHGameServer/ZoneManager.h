#pragma once
#include "Zone.h"
#include "ObjectPool.h"
#include "Monster.h"

class ClientPacketHandler;

enum ZoneIDEnum
{
	START_ZONE,
	FIELD_ZONE,

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
	void TickUpdate();

public:
	std::vector<ZonePtr> zoneVector;
	std::shared_ptr<ObjectPool<Zone>> zonePool;
	uint32 numOfActors;

public:
	RecursiveMutex zoneLock;
	void SpawnNpc();
};

extern ZoneManager GZoneManager;
