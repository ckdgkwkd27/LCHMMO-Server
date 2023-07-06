#pragma once
class ClientSession;

class Player
{
public:
	uint32 playerId;
	std::string name;
	std::shared_ptr<ClientSession> ownerSession;
};

using PlayerPtr = std::shared_ptr<Player>;
