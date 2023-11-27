#pragma once
#include "Actor.h"
#include "Player.h"
#include "Map.h"
#include "MessageQueue.h"
#include "ClientPacketHandler.h"
using ZoneIDType = uint32;

class Zone
{
public:
	void Init();
	void RegisterActor(ActorPtr _actor);
	void UnregisterActor(ActorPtr _actor);
	ActorPtr FindActor(ActorIDType _actorId);
	PlayerPtr FindPlayerInCondition(std::function<bool(ActorPtr)> _condition);
	void BroadCast(ActorPtr _selfPlayer, CircularBufferPtr _sendBuffer);

	bool Update();

	void EnterGame(PlayerPtr player, ZoneIDType zoneId = 0);
	void LeaveGame(ActorIDType _actorId);
	void HandleMove(PlayerPtr player, protocol::RequestMove movePacket);
	void HandleSkill(PlayerPtr player, protocol::RequestSkill packet);

public:
	ZoneIDType zoneID;
	std::vector<ActorPtr> actorVector;
	int32 xMax;
	int32 xMin;
	int32 yMax;
	int32 yMin;
	Map zoneMap;
	MessageQueue messageQueue;

private:
	RecursiveMutex actorLock;
};

using ZonePtr = std::shared_ptr<Zone>;
