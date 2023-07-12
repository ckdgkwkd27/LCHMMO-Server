#pragma once
#include "Session.h"
#include "SessionManager.h"

enum SessionState : uint8
{
	NONE,
	CONNECTED,
	LOGIN,
	ENTER_GAME,

	END
};

class Player;
class ClientSession : public Session
{
public:
	virtual void OnAccepted();
	virtual void OnConnected();
	virtual void OnDisconnected();
	virtual void OnSend(uint32 len);
	virtual uint32 OnRecv(char* buffer, uint32 len);

public:
	std::shared_ptr<Player> currentPlayer;
};

