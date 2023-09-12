#pragma once
#include "protocol.pb.h"
using ActorIDType = uint64;

enum class ObjectType : uint32
{
	NONE = 0,
	PLAYER,
	MONSTER,
	PROJECTILE
};

enum class MoveState : uint32
{
	IDLE = 0,
	MOVING = 1,
	SKILL = 2,
	DEAD = 3,
};

class Actor
{
public:
	virtual ~Actor() {}
	virtual void Update() {}

public:
	protocol::ObjectInfo ActorInfo;
	uint32 zoneID;
};

using ActorPtr = std::shared_ptr<Actor>;
