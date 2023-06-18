#include "pch.h"
#include "SessionManager.h"
#include "IocpServer.h"

SessionManager GSessionManager;

SessionPtr SessionManager::CreateSession(IocpCorePtr _iocpCore, SessionFactory _factory)
{
    auto _session = _factory();
    //_iocpCore->Register(_session);
    return _session;
}

void SessionManager::AddSession(SessionPtr _session)
{
    LockGuard lockGuard(sessionLock);
    //sessions.insert(_session);
    userCnt++;
}

void SessionManager::RemoveSession(SessionPtr _session)
{
    LockGuard lockGuard(sessionLock);
    //sessions.erase(_session);
    userCnt--;
}

void SessionManager::Broadcast(CircularBuffer _sendBuffer)
{
    LockGuard lockGuard(sessionLock);
    for (auto _session : activeSessions)
    {
        //_session->PostSend();
    }
}

bool SessionManager::AcceptClientSession()
{
    while (GetIssueCount() < GIocpServer->GetMaxConnectionCnt())
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

void SessionManager::PrepareSessions(uint32 maxSessionCnt)
{
    for (uint32 i = 0; i < maxSessionCnt; i++)
    {
        SessionPtr _session = std::make_shared<Session>();
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
