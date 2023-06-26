#pragma once
#include "Session.h"
#include "ServerPacketHandler.h"

class ServerSession : public Session
{
public:
	virtual void OnAccepted();
	virtual void OnConnected();
	virtual void OnDisconnected();
	virtual void OnSend(uint32 len);
	virtual uint32 OnRecv(char* buffer, uint32 len);
};

