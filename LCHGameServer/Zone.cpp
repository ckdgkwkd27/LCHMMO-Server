#include "pch.h"
#include "Zone.h"

void Zone::RegisterActor(Actor* _actor)
{
	actorVector.push_back(_actor);
}

Actor* Zone::FindActor(ActorIDType _actorID)
{
	auto it = std::find_if(actorVector.begin(), actorVector.end(), [_actorID](Actor* _actor) { return _actor->actorID == _actorID;});
	if (it == actorVector.end())
		return nullptr;

	return *it;
}
