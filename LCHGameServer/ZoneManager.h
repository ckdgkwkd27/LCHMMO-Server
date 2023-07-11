#pragma once
#include "Zone.h"
#include "ObjectPool.h"

class ZoneManager
{
public:
	ZoneManager();
	void Initialize();
	void RegisterZone(Zone* _zone);
	bool RegisterActor(ZoneIDType _zoneId, Actor* _actor);
	Zone* FindZoneByID(ZoneIDType _zoneId);

public:
	std::vector<Zone*> zoneVector;
	ObjectPool<Zone>* zonePool;
};

extern ZoneManager GZoneManager;
