#pragma once
#include "Actor.h"
using PlayerIDType = uint64;

class ClientSession;

class Player : public Actor
{
public:
	PlayerIDType playerId;
	std::shared_ptr<ClientSession> ownerSession;
};

using PlayerPtr = std::shared_ptr<Player>;
