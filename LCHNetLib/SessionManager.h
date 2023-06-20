#pragma once
#include "Session.h"
#include "IocpCore.h"
#include "CircularBuffer.h"

class IocpServer;

class SessionManager
{
private:
	std::list<SessionPtr> activeSessions;
	std::list<SessionPtr> sessionPool;

	RecursiveMutex sessionLock;

	uint32 userCnt = 0; //À¯Àú ¼ö
	uint32 issueCnt = 0;

public:
	SessionPtr CreateSession(IocpCorePtr _iocpCore, SessionFactory _factory);
	void Broadcast(CircularBufferPtr _sendBuffer);

	bool AcceptClientSession();
	void PrepareSessions(uint32 maxSessionCnt);
	SessionPtr IssueSession();
	void ReturnSession(SessionPtr _session);

	uint32 GetIssueCount() { return issueCnt; }
	std::list<SessionPtr> GetSessionPool() { return sessionPool; }
};

extern SessionManager GSessionManager;

