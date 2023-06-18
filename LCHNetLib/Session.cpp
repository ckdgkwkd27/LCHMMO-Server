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

void Session::Register()
{
    CreateIoCompletionPort(reinterpret_cast<HANDLE>(sessionSocket), GIocpServer->GetIocpHandle(), 0, 0);
}

bool Session::PostSend(CircularBuffer _sendBuffer)
{
    if (isConnected == false)
        return false;

    {
        LockGuard lockGuard(sendLock);

        sessionSendEvent.Init();
        sessionSendEvent.sessionRef = shared_from_this();
        sessionSendEvent.sendBuffer = _sendBuffer;

        DWORD numOfBytes = 0;
        WSABUF wsabuf;
        wsabuf.buf = sessionSendEvent.sendBuffer->data();
        wsabuf.len = static_cast<LONG>(sessionSendEvent.sendBuffer->DataSize());

        if (SOCKET_ERROR == WSASend(_socket, &wsabuf, (DWORD)1, &numOfBytes, 0, &sessionSendEvent, nullptr))
        {
            INT32 errCode = WSAGetLastError();
            if (errCode != WSA_IO_PENDING) {
                std::cout << "[FAIL] Send Error: " << errCode << std::endl;
                sessionSendEvent.sessionRef = nullptr;
                sessionSendEvent.sendBuffer->Clear();
            }
        }

    }
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
	sessionAcceptEvent.sessionRef = shared_from_this();
    sessionAcceptEvent.Init();

	DWORD bytes = 0;
	if (false == AcceptEx(GIocpServer->GetListenSocket(), sessionSocket, AcceptBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		&bytes, static_cast<LPOVERLAPPED>(&sessionAcceptEvent)))
	{
		const INT32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			std::cout << "[FAIL] PostAccept() Error: " << errCode << std::endl;
			PostAccept();
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

bool Session::ProcessAccept()
{
    isConnected = true;
    SocketUtil::SetOptionUpdateAcceptSocket(sessionSocket, GIocpServer->GetListenSocket());
    SocketUtil::SetOptionNoDelay(sessionSocket, true);
    SocketUtil::SetOptionLinger(sessionSocket);

    
    SOCKADDR_IN _sockAddrIn;
    int32 AddrSize = sizeof(_sockAddrIn);
    SOCKADDR* castSockAddr = reinterpret_cast<SOCKADDR*>(&_sockAddrIn);

    if (SOCKET_ERROR == ::getpeername(sessionSocket, castSockAddr, &AddrSize))
    {
        std::cout << "[FAIL] getpeername error: " << GetLastError() << std::endl;
        return false;
    }

    Register();
    sockAddrIn = _sockAddrIn;
    std::cout << "[INFO] Accept Completed" << std::endl;
    return true;
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
