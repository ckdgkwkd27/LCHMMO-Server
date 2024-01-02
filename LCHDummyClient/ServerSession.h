#pragma once
#include "Session.h"
#include "SessionManager.h"

enum SessionState : uint8
{
	NONE,
	CONNECTED,
	LOGIN,
	ENTER_GAME,
	GAME_READY,

	END
};

class ServerSession : public Session
{
public:
	virtual void OnAccepted();
	virtual void OnConnected();
	virtual void OnDisconnected();
	virtual uint32 OnRecv(char* buffer, uint32 len);

public:
	uint8 state = NONE;
	uint32 playerId;
	uint32 actorId;
};
