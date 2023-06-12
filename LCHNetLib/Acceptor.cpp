#include "pch.h"
#include "Acceptor.h"
#include "SocketUtil.h"

Acceptor::Acceptor(uint32 _maxSessionCnt, SessionFactory _onAcceptCompleted, Wstring _ip, uint16 _port)
{
    SocketUtil::Init();
    acceptSocket = SocketUtil::CreateSocket();

    iocpCore = std::make_shared<IocpCore>();
    ip = _ip;
    port = _port;
    maxSessionCnt = _maxSessionCnt;
    onAcceptCompleted = _onAcceptCompleted;
}

Acceptor::~Acceptor()
{
    SocketUtil::CloseSocket(acceptSocket);
    for (AcceptEvent* _event : acceptEvents)
    {
        delete(_event);
    }
}

bool Acceptor::StartAccept()
{
	acceptSocket = SocketUtil::CreateSocket();
	if(acceptSocket == INVALID_SOCKET)
		return false;
    
    RETURN_FALSE_ON_FAIL(GetIocpCore()->Register(shared_from_this()));
    RETURN_FALSE_ON_FAIL(SocketUtil::SetOptionReuseAddr(acceptSocket, true));
    RETURN_FALSE_ON_FAIL(SocketUtil::SetOptionLinger(acceptSocket));
    RETURN_FALSE_ON_FAIL(SocketUtil::Bind(acceptSocket, ip, port));
    RETURN_FALSE_ON_FAIL(SocketUtil::Listen(acceptSocket));

	const uint32 acceptCount = maxSessionCnt;
	for (uint32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = new AcceptEvent();
		acceptEvent->owner = shared_from_this();
		acceptEvents.push_back(acceptEvent);
		PostAccept(acceptEvent);
	}

	return true;
}

bool Acceptor::PostAccept(AcceptEvent* _acceptEvent)
{
	auto _session = GSessionManager.CreateSession(iocpCore, onAcceptCompleted);
	SOCKET skt = _session->GetSocket();

    _acceptEvent->sessionRef = _session;
    _acceptEvent->Init();

	DWORD bytesReceived = 0;

	if (false == AcceptEx(acceptSocket, _session->GetSocket(),
        _session->recvBuffer.WritePos(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		&bytesReceived, static_cast<LPOVERLAPPED>(_acceptEvent)))
	{
		const INT32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING) {
			std::cout << "PostAccept_ERRCODE: " << errCode << std::endl;
			PostAccept(_acceptEvent);
		}
	}

    return true;
}

HANDLE Acceptor::GetHandle()
{
    return reinterpret_cast<HANDLE>(acceptSocket);
}

bool Acceptor::Dispatch(IocpEvent* _iocpEvent, int32 bytes)
{
    if (_iocpEvent->GetType() != EventType::ACCEPT)
        return false;

    bytes = 0;
    AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(_iocpEvent);
    SessionPtr session = acceptEvent->sessionRef;

    if (false == SocketUtil::SetOptionUpdateAcceptSocket(session->GetSocket(), acceptSocket))
    {
        PostAccept(acceptEvent);
        std::cout << "[ERROR] Socket Option(SO_UPDATE_ACCEPT_CONTEXT) Failed!" << std::endl;
        return false;
    }

    SOCKADDR_IN sockAddr;
    size_t AddrSize = sizeof(sockAddr);
    if (SOCKET_ERROR == ::getpeername(session->GetSocket(), reinterpret_cast<SOCKADDR*>(&sockAddr), reinterpret_cast<int*>(AddrSize)))
    {
        PostAccept(acceptEvent);
        return false;
    }

    session->sockAddr = sockAddr;
    session->ProcessConnect();
    PostAccept(acceptEvent);
    return true;
}
