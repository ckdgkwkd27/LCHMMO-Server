#pragma once
#include "Actor.h"
class ClientSession;

class Player : public Actor
{
public:
	uint32 playerId;
	std::string name;
	std::shared_ptr<ClientSession> ownerSession;
};

using PlayerPtr = std::shared_ptr<Player>;
