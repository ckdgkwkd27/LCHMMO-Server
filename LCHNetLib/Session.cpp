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
