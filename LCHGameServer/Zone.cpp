#include "pch.h"
#include "Zone.h"
#include "Player.h"
#include "ClientSession.h"

void Zone::Init()
{
	zoneMap.LoadMap(zoneID);
	this->xMax = zoneMap.MaxX;
	this->xMin = zoneMap.MinX;
	this->yMax = zoneMap.MaxY;
	this->yMin = zoneMap.MinY;

	//Spawn Monster or Npc
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
