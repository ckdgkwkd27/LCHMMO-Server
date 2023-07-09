#pragma once
#define MAX_PLAYER 65535
#include "Player.h"

class PlayerManager {
public:
	void Init(uint64 maxSize = MAX_PLAYER);
	PlayerPtr NewPlayer();
	void DeletePlayer(PlayerPtr _player);
	PlayerPtr FindPlayerByName(std::string _name);

private:
	void AddPlayer(PlayerPtr _player);
	void ReturnPlayer(PlayerPtr _player);

public:
	std::unordered_map<uint64, PlayerPtr> AllPlayerInfo;

private:
	RecursiveMutex playerLock;
	std::queue<PlayerPtr> playerPool;
	uint64 NumOfPlayers = 0;
	uint64 PlayerIdx = 0;

public:
	uint64 GetPlayerIdx() { return PlayerIdx; }
	void IncreasePlayerIdx() { PlayerIdx++; }
	void DecreasePlayerIdx() { PlayerIdx--; }
};

extern PlayerManager GPlayerManager;

