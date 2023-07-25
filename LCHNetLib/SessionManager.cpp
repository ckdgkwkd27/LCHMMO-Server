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
    while (issueCnt < maxSessionCnt)
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

bool SessionManager::ConnectServerSession(uint32 maxSessionCnt, Wstring connIp, uint32 connPort)
{
    while (issueCnt < maxSessionCnt)
    {
        SessionPtr _session = IssueSession();
        if (_session == nullptr)
        {
            return false;
        }

        _session->Register();
        if (_session->PostConnect(connIp, connPort) == false)
        {
            return false;
        }
    }

    return true;
}

SessionPtr SessionManager::IssueSession()
{
    LockGuard lockGuard(sessionLock);
    if (sessionPool.size() == 0)
    {
        return nullptr;
    }

    issueCnt++;

    SessionPtr _session = sessionPool.back();
    if (_session == nullptr)
    {
        return nullptr;
    }

    sessionPool.pop_back();

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

void SessionManager::AddToActivePool(SessionPtr _session)
{
    LockGuard lockGuard(sessionLock);
    CRASH_ASSERT((_session != nullptr));

    activeSessions.push_back(_session);
}

void SessionManager::DeleteFromActivePool(SessionPtr _session)
{
    LockGuard lockGuard(sessionLock);
    CRASH_ASSERT((_session != nullptr));
    CRASH_ASSERT((activeSessions.end() != std::find(activeSessions.begin(), activeSessions.end(), _session)));
}
