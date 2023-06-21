#include "pch.h"
#include "SessionManager.h"

SessionManager GSessionManager;

SessionPtr SessionManager::CreateSession(IocpCorePtr _iocpCore, SessionFactory _factory)
{
    auto _session = _factory();
    return _session;
}

void SessionManager::Broadcast(CircularBufferPtr _sendBuffer)
{
    LockGuard lockGuard(sessionLock);
    for (auto _session : activeSessions)
    {
        _session->PostSend(_sendBuffer);
    }
}

bool SessionManager::AcceptClientSession(uint32 maxSessionCnt)
{
    while (GetIssueCount() < maxSessionCnt)
    {
        SessionPtr _session = IssueSession();
        if (_session == nullptr)
        {
            return false;
        }

        if (_session->PostAccept() == false)
        {
            return false;
        }
    }

    return true;
}

void SessionManager::PrepareSessions(uint32 maxSessionCnt, SOCKET _listenSocket, HANDLE _iocpHandle)
{
    for (uint32 i = 0; i < maxSessionCnt; i++)
    {
        SessionPtr _session = std::make_shared<Session>();
        _session->SetListenSocket(_listenSocket);
        _session->SetIocpHandle(_iocpHandle);
        sessionPool.push_back(_session);
    }
}

SessionPtr SessionManager::IssueSession()
{
    LockGuard lockGuard(sessionLock);
    issueCnt++;

    SessionPtr _session = sessionPool.back();
    if (_session == nullptr)
    {
        return nullptr;
    }

    sessionPool.pop_back();
    activeSessions.push_back(_session);

    return _session;
}

void SessionManager::ReturnSession(SessionPtr _session)
{
    LockGuard lockGuard(sessionLock);
    issueCnt--;

    for (auto it = activeSessions.begin(); it != activeSessions.end(); it++)
    {
        if (*it == _session)
        {
            activeSessions.erase(it);
        }
    }

    sessionPool.push_back(_session);
}
