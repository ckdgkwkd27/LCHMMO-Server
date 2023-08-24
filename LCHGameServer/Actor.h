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

struct PositionType
{
	int32 X;
	int32 Y;
};

struct RotationType
{
	int32 X;
	int32 Y;
};

class Actor
{
public:
	virtual ~Actor() {}
public:
	protocol::ObjectInfo ActorInfo;
};

using ActorPtr = std::shared_ptr<Actor>;
