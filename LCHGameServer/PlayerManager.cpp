#include "pch.h"
#include "PlayerManager.h"
PlayerManager GPlayerManager;

void PlayerManager::Init(uint64 maxSize)
{
	for (uint32 idx = 0; idx < maxSize; idx++)
	{
		PlayerPtr _player = std::make_shared<Player>();
		_player->playerId = idx;
		playerPool.push(_player);
	}
	NumOfPlayers = 0;

	std::cout << "[INFO] Player Init Completed!" << std::endl;
}

PlayerPtr PlayerManager::NewPlayer()
{
	LockGuard playerGuard(playerLock);
	PlayerPtr _player = playerPool.front();
	playerPool.pop();
	AddPlayer(_player);
	IncreasePlayerIdx();
	return _player;
}

void PlayerManager::AddPlayer(PlayerPtr _player)
{
	AllPlayerInfo.insert({PlayerIdx, _player});
	NumOfPlayers++;
}

void PlayerManager::DeletePlayer(PlayerPtr _player)
{
	LockGuard playerGuard(playerLock);
	uint64 _playerId = _player->playerId;
	AllPlayerInfo.erase(_playerId);
	ReturnPlayer(_player);
}

PlayerPtr PlayerManager::FindPlayerByName(std::string _name)
{
	uint64 _playerId;
	for (auto& info : AllPlayerInfo)
	{
		if (info.second->name == _name)
		{
			_playerId = info.first;
			return info.second;
		}
	}
	return nullptr;
}


void PlayerManager::ReturnPlayer(PlayerPtr _player)
{
	playerPool.push(_player);
	NumOfPlayers--;
}
