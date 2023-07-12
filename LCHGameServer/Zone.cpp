#include "pch.h"
#include "Zone.h"

void Zone::RegisterActor(ActorPtr _actor)
{
	if(_actor == nullptr)
		ASSERT_CRASH(false);
	actorVector.push_back(_actor);
}

ActorPtr Zone::FindActor(ActorIDType _actorID)
{
	auto it = std::find_if(actorVector.begin(), actorVector.end(), [_actorID](ActorPtr _actor)
		{
			if (_actor == nullptr) ASSERT_CRASH(false);
			return _actor->actorID == _actorID;
		});
	if (it == actorVector.end())
		return nullptr;

	return *it;
}
