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

bool Session::PostSend(CircularBufferPtr _sendBuffer)
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

        if (SOCKET_ERROR == WSASend(sessionSocket, &wsabuf, (DWORD)1, &numOfBytes, 0, &sessionSendEvent, nullptr))
        {
            INT32 errCode = WSAGetLastError();
            if (errCode != WSA_IO_PENDING) 
            {
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
    if (isConnected == false)
        return false;

    sessionRecvEvent.Init();
    sessionRecvEvent.sessionRef = shared_from_this();

    WSABUF wsaBuf;
    wsaBuf.buf = recvBuffer.WritePos();
    wsaBuf.len = recvBuffer.FreeSize();

	DWORD recvBytes = 0;
	DWORD flags = 0;
    if (SOCKET_ERROR == WSARecv(sessionSocket, &wsaBuf, (DWORD)1, &recvBytes, &flags, &sessionRecvEvent, NULL))
    {
        int32 ErrorCode = WSAGetLastError();
        if (ErrorCode != WSA_IO_PENDING)
        {
            sessionRecvEvent.sessionRef = nullptr;
            std::cout << "[FAIL] PostRecv Error: " << ErrorCode << std::endl;
            return false;
        }
    }

    return true;
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
    if (SOCKET_ERROR == ::getpeername(sessionSocket, reinterpret_cast<sockaddr*>(&_sockAddrIn), &AddrSize))
    {
        std::cout << "[FAIL] getpeername error: " << GetLastError() << std::endl;
        return false;
    }

    Register();
    sockAddrIn = _sockAddrIn;
    char ipBuf[32];

    inet_ntop(AF_INET, &_sockAddrIn.sin_addr.s_addr, ipBuf, sizeof(ipBuf));
    std::cout << "[INFO] Session Accept Completed: " << ipBuf << std::endl;

    PostRecv();

    sendBuffer.WriteBuf(const_cast<char*>("Hello World123456789"));
    PostSend(std::make_shared<CircularBuffer>(sendBuffer));
    return true;
}

bool Session::ProcessSend(int32 bytes)
{
    sessionSendEvent.sessionRef = nullptr;
    sessionSendEvent.sendBuffer->Clear();

    if (bytes == 0)
    {
        PostDisconnect();
        return true;
    }

    OnSend(bytes);
    return true;
}

bool Session::ProcessRecv(int32 bytes)
{
    if (bytes == 0)
    {
        PostDisconnect();
        return false;
    }

    if (recvBuffer.OnWrite(bytes) == false)
    {
        PostDisconnect();
        return false;
    }

    uint32 dataSize = recvBuffer.DataSize();
    uint32 processLen = OnRecv(recvBuffer.ReadPos(), dataSize);
    if (processLen < 0 || dataSize < processLen || recvBuffer.OnRead(processLen) == false)
    {
        PostDisconnect();
        return false;
    }

    std::cout << recvBuffer.data() << std::endl;

    recvBuffer.Reposition();
    PostRecv();
    return true;
}

bool Session::ProcessConnect()
{
    return false;
}

bool Session::ProcessDisconnect()
{
    return false;
}
