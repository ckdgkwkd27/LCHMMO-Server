#include "pch.h"
#include "ZoneManager.h"

ZoneManager GZoneManager;

ZoneManager::ZoneManager()
{
	zonePool = new ObjectPool<Zone>(10);
}

//#TODO 추후에 Config에서 읽을수 있도록한다
void ZoneManager::Initialize()
{
	for (uint32 idx = 0; 5; idx++)
	{
		Zone* _zone = zonePool->BorrowObject();
		_zone->zoneID = idx;
		_zone->xMax = 100;
		_zone->yMax = 100;
		RegisterZone(_zone);
	}
}

void ZoneManager::RegisterZone(Zone* _zone)
{
	zoneVector.push_back(_zone);
}

bool ZoneManager::RegisterActor(ZoneIDType _zoneID, Actor* _actor)
{
	Zone* _zone = nullptr;
	auto it = zoneVector.begin();
	for (; it != zoneVector.end(); it++)
	{
		if ((*it)->zoneID == _zoneID)
		{
			_zone = *it;
			break;
		}
	}

	//그런 존이 없음
	if (it == zoneVector.end())
	{
		return false;
	}

	//이미 액터가 존재
	if (nullptr != (*it)->FindActor(_actor->actorID))
	{
		return false;
	}
	
	(*it)->RegisterActor(_actor);
	return true;
}
