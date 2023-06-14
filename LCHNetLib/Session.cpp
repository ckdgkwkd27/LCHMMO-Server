#include "pch.h"
#include "Session.h"
#include "IocpServer.h"

Session::Session()
    : recvBuffer(BUFFER_SIZE), sendBuffer(BUFFER_SIZE), isConnected(false)
{
    sessionSocket = SocketUtil::CreateSocket();
}

Session::~Session()
{
    SocketUtil::CloseSocket(sessionSocket);
}

bool Session::PostSend(const char* buffer, size_t len)
{
    if (isConnected == false)
        return false;

    LockGuard lockGuard(sendLock);

    return true;
}

bool Session::FlushSend()
{
    return true;
}

bool Session::PreRecv()
{
    return false;
}

bool Session::PostRecv()
{
    return false;
}

bool Session::PostAccept()
{
    return false;
}

bool Session::PostConnect()
{
    return false;
}

bool Session::PostDisconnect()
{
    return false;
}

bool Session::ProcessAccept()
{
    return false;
}

bool Session::ProcessSend(int32 bytes)
{
    return false;
}

bool Session::ProcessRecv(int32 bytes)
{
    return false;
}

bool Session::ProcessConnect()
{
    return false;
}

bool Session::ProcessDisconnect()
{
    return false;
}

HANDLE Session::GetHandle()
{
    return HANDLE();
}

bool Session::Dispatch(IocpEvent* _iocpEvent, int32 bytes)
{
    return true;
}
