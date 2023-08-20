#pragma once
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
	int32 Z;
};

struct RotationType
{
	int32 X;
	int32 Y;
	int32 Z;
};

class Actor
{
public:
	virtual ~Actor() {}
public:
	ActorIDType actorId;
	PositionType position;
	RotationType rotation;
};

using ActorPtr = std::shared_ptr<Actor>;
