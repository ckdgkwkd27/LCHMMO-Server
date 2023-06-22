#pragma once
#include "Session.h"
#include "SessionManager.h"

class ClientSession : public Session
{
public:
	virtual void OnConnected();
	virtual void OnDisconnected();
	virtual void OnSend(uint32 len);
	virtual uint32 OnRecv(char* buffer, uint32 len);
};

