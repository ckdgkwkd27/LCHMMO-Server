#pragma once
using ActorIDType = uint32;

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
