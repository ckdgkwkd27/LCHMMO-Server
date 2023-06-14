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
	void AddSession(SessionPtr _session);
	void RemoveSession(SessionPtr _session);
	void Broadcast(CircularBuffer _sendBuffer);

	bool AcceptClientSession();
	void PrepareSessions(uint32 maxSessionCnt);
	SessionPtr IssueSession();
	void ReturnSession(SessionPtr _session);

	uint32 GetIssueCount() { return issueCnt; }
};

extern SessionManager GSessionManager;

