#include "pch.h"
#include "ZoneManager.h"

ZoneManager GZoneManager;

ZoneManager::ZoneManager()
{
	zonePool = std::make_shared<ObjectPool<Zone>>(10);
	numOfActors = 0;
}

//#TODO 추후에 Config에서 읽을수 있도록한다
void ZoneManager::Initialize()
{
	for (uint32 idx = 0; idx < 5; idx++)
	{
		ZonePtr _zone = ZonePtr(zonePool->BorrowObject());
		_zone->zoneID = idx;
		_zone->xMax = 100;
		_zone->yMax = 100;
		RegisterZone(_zone);
	}

	std::cout << "[INFO] Zone Init Completed!" << std::endl;
}

void ZoneManager::RegisterZone(ZonePtr _zone)
{
	LockGuard guard(zoneLock);
	zoneVector.push_back(_zone);
}

bool ZoneManager::RegisterActor(ZoneIDType _zoneID, ActorPtr _actor)
{
	LockGuard guard(zoneLock);

	ZonePtr _zone = nullptr;
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
	if (nullptr != (*it)->FindActor(_actor->actorId))
	{
		return false;
	}
	
	(*it)->RegisterActor(_actor);
	return true;
}

ActorIDType ZoneManager::IssueActorID()
{
	LockGuard guard(zoneLock);
	uint32 newActorID = numOfActors;
	numOfActors++;
	return newActorID;
}

ZonePtr ZoneManager::FindZoneByID(ZoneIDType _zoneId)
{
	LockGuard guard(zoneLock);
	auto it = std::find_if(zoneVector.begin(), zoneVector.end(), [_zoneId](ZonePtr _zone) { return _zone->zoneID == _zoneId; });
	if(it == zoneVector.end())
		return nullptr;

	return *it;
}
