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
    AcceptEvent* acceptEvent = new AcceptEvent();
    DWORD bytes = 0;
    DWORD flags = 0;
    acceptContext->mWsaBuf.len = 0;
    acceptContext->mWsaBuf.buf = nullptr;

    if (FALSE == AcceptEx(GIocpServer->GetListenSocket(), sessionSocket, GIocpServer->AcceptBuf, 0,
        sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytes, (LPOVERLAPPED)acceptContext))
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            DeleteIoContext(acceptContext);
            printf_s("AcceptEx Error : %d\n", GetLastError());

            return false;
        }
    }

    return true;
}

bool Session::PostConnect()
{
    return false;
}

bool Session::PostDisconnect()
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
