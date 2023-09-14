#include "pch.h"
#include "Zone.h"
#include "Player.h"
#include "ClientSession.h"
#include "Monster.h"

void Zone::Init()
{
	zoneMap.LoadMap(zoneID);
	this->xMax = zoneMap.MaxX;
	this->xMin = zoneMap.MinX;
	this->yMax = zoneMap.MaxY;
	this->yMin = zoneMap.MinY;

	//Spawn Monster or Npc
	if (zoneID == 0) //#TODO Set to Zone1
	{
		MonsterPtr monster = std::make_shared<Monster>();
		monster->ActorInfo.mutable_posinfo()->set_posx(5);
		monster->ActorInfo.mutable_posinfo()->set_posy(5);
		RegisterActor(monster);
		std::cout << "[TEMPLOG] Monster Spawned!" << std::endl;
	}
}

void Zone::RegisterActor(ActorPtr _actor)
{
	LockGuard guard(actorLock);

	if(_actor == nullptr)
		CRASH_ASSERT(false);
	actorVector.push_back(_actor);
}

ActorPtr Zone::FindActor(ActorIDType _actorID)
{
	LockGuard guard(actorLock);

	auto it = std::find_if(actorVector.begin(), actorVector.end(), [_actorID](ActorPtr _actor)
		{
			if (_actor == nullptr) CRASH_ASSERT(false);
			return _actor->ActorInfo.actorid() == _actorID;
		});
	if (it == actorVector.end())
		return nullptr;

	return *it;
}

PlayerPtr Zone::FindPlayerInCondition(std::function<bool(ActorPtr)> _condition)
{
	LockGuard guard(actorLock);

	for (ActorPtr _actor : actorVector)
	{
		if (_actor->ActorInfo.objecttype() == (uint32)ObjectType::PLAYER)
		{
			if (_condition(_actor) == true)
			{
				return std::dynamic_pointer_cast<Player>(_actor);
			}
		}
	}
	return nullptr;
}

void Zone::BroadCast(ActorPtr _selfPlayer, CircularBufferPtr _sendBuffer)
{
	LockGuard guard(actorLock);

	for (auto& _actor : actorVector)
	{
		PlayerPtr _player = nullptr;
		_player = std::dynamic_pointer_cast<Player>(_actor);
		if (_player != nullptr && _player != _selfPlayer)
		{
			printf("ME=%lld => OTHER=%lld\n", _selfPlayer->ActorInfo.actorid(), _player->ActorInfo.actorid());
			printf("_player SOCKET=%lld", _player->ownerSession->GetSocket());
			_player->ownerSession->PostSend(_sendBuffer);
		}
	}
}

bool Zone::Update(milliseconds UpdateTimeStamp)
{
	for (ActorPtr _actor : actorVector)
	{
		_actor->Update(UpdateTimeStamp);
	}

	return true;
}
