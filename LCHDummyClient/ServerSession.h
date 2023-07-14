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

class ServerSession : public Session
{
public:
	virtual void OnAccepted();
	virtual void OnConnected();
	virtual void OnDisconnected();
	virtual uint32 OnRecv(char* buffer, uint32 len);

public:
	uint8 state = NONE;
	uint64 playerId;
};
