#include "pch.h"
#include "SessionManager.h"

SessionManager GSessionManager;

SessionPtr SessionManager::CreateSession(IocpCorePtr _iocpCore, SessionFactory _factory)
{
    auto _session = _factory();
    _iocpCore->Register(_session);
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
    for (auto _session : sessions)
    {
        //_session->PostSend();
    }
}

bool SessionManager::AcceptClientSession()
{
    LockGuard lockGuard(sessionLock);
    auto _session = sessions.back();
    sessions.pop_back();

    ++issueCnt;
    if(_session->PostAccept() == false)
        return false;

    return true;
}
