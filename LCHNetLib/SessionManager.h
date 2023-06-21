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

	bool AcceptClientSession(uint32 maxSessionCnt);

	template <typename T>
	void PrepareSessions(uint32 maxSessionCnt, SOCKET _listenSocket, HANDLE _iocpHandle);
	SessionPtr IssueSession();
	void ReturnSession(SessionPtr _session);

	uint32 GetIssueCount() { return issueCnt; }
	std::list<SessionPtr> GetSessionPool() { return sessionPool; }
};

extern SessionManager GSessionManager;

template <typename T>
void SessionManager::PrepareSessions(uint32 maxSessionCnt, SOCKET _listenSocket, HANDLE _iocpHandle)
{
	for (uint32 i = 0; i < maxSessionCnt; i++)
	{
		SessionPtr _session = std::make_shared<T>();
		_session->SetListenSocket(_listenSocket);
		_session->SetIocpHandle(_iocpHandle);
		sessionPool.push_back(_session);
	}
}