#include "pch.h"
#include "Viewport.h"
#include "ZoneManager.h"

Viewport::Viewport(PlayerPtr _owner)
{
    this->owner = _owner;
}

std::set<ActorPtr> Viewport::GatherActors()
{
    if (owner == nullptr || owner->zoneID == 0)
        return std::set<ActorPtr>();

    std::set<ActorPtr> actors;
    ZonePtr zone = GZoneManager.FindZoneByID(owner->zoneID);

    Vector2Int ownerCellpos = Vector2Int::GetVectorFromActorPos(owner->ActorInfo.posinfo());
    std::set<SectionPtr> sections = zone->GetAdjacentSections(ownerCellpos);
    
    for (SectionPtr _section : sections)
    {
        for (ActorPtr _actor : _section->actorVector)
        {
            Vector2Int actorCellPos = Vector2Int::GetVectorFromActorPos(_actor->ActorInfo.posinfo());
            int32 dx = actorCellPos.x - ownerCellpos.x;
            int32 dy = actorCellPos.y - ownerCellpos.y;
            if (abs(dx) > VIEWPORT_CELL)
                continue;
			if (abs(dy) > VIEWPORT_CELL)
				continue;
            actors.insert(_actor);
        }
    }

    return actors;
}

void Viewport::Update()
{
    if (owner == nullptr || owner->zoneID == 0)
        return;

    std::set<ActorPtr> currActors = GatherActors();

    std::vector<ActorPtr> addedActors;
    {
		for (auto _actor : currActors)
		{
			if (std::find(prevActors.begin(), prevActors.end(), _actor) == prevActors.end())
				addedActors.push_back(_actor);
		}

		if (addedActors.size() > 0)
		{
			protocol::NotifySpawn spawnPkt;
			for (auto _actor : addedActors)
			{
				protocol::ObjectInfo* info = spawnPkt.add_objects();
				info->MergeFrom(_actor->ActorInfo);
			}

			auto _sendBuffer = ClientPacketHandler::MakeSendBufferPtr(spawnPkt);
			owner->ownerSession->PostSend(_sendBuffer);
		}
    }

    std::vector<ActorPtr> removedActors;
    {
        for (auto _actor : prevActors)
        {
            if (std::find(currActors.begin(), currActors.end(), _actor) == currActors.end())
                removedActors.push_back(_actor);
        }

        if (removedActors.size() > 0)
        {
            protocol::NotifyDespawn despawnPkt;
            for (auto _actor : removedActors)
            {
                despawnPkt.add_actorids(_actor->ActorInfo.actorid());
            }

            auto _sendBuffer = ClientPacketHandler::MakeSendBufferPtr(despawnPkt);
            owner->ownerSession->PostSend(_sendBuffer);
        }
    }
}