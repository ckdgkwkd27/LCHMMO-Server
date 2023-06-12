#pragma once
#include "Session.h"
#include "IocpCore.h"
#include "CircularBuffer.h"

class SessionManager
{
private:
	std::list<SessionPtr> sessions;
	RecursiveMutex sessionLock;

	uint32 userCnt = 0; //À¯Àú ¼ö
	uint32 issueCnt;
	uint32 returnCnt;

public:
	SessionPtr CreateSession(IocpCorePtr _iocpCore, SessionFactory _factory);
	void AddSession(SessionPtr _session);
	void RemoveSession(SessionPtr _session);
	void Broadcast(CircularBuffer _sendBuffer);

	bool AcceptClientSession();
};

extern SessionManager GSessionManager;

