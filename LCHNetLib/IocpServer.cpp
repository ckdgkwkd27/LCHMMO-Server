#include "pch.h"
#include "IocpServer.h"

IocpServer* GIocpServer = nullptr;
char AcceptBuf[64] = { 0, };

IocpServer::IocpServer(std::wstring _ip, uint16 _port, uint32 _maxConnectionCnt)
{
	ip = _ip;
	port = _port;
	maxConnectionCnt = _maxConnectionCnt;
}

void IocpServer::Initialize()
{
	uint32 workerThreadCnt = std::thread::hardware_concurrency();
	workerThreads.resize(workerThreadCnt);

	iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	listenSocket = SocketUtil::CreateListenSocket(ip, port);
	SocketUtil::Listen(listenSocket);

	std::cout << "Server Initialization Success.." << std::endl;
	return;
}

bool IocpServer::Run()
{
	StartAccept();

	for (uint32 i = 0; i < workerThreadsCnt; i++)
	{
		workerThreads[i] = std::thread(&IocpServer::WorkerThreadFunc, this);
	}

	return true;
}

bool IocpServer::Join()
{
	for (uint32 i = 0; i < workerThreadsCnt; i++)
	{
		if (workerThreads[i].joinable())
		{
			workerThreads[i].join();
		}
	}

	return true;
}

void IocpServer::StartAccept()
{
	while (GSessionManager.AcceptClientSession())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void IocpServer::WorkerThreadFunc()
{
	while (true)
	{
		DWORD bytes;
		ULONG_PTR key;
		IocpEvent* iocpEvent = nullptr;
		bool ret = GetQueuedCompletionStatus(iocpHandle, &bytes, &key, reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), INFINITE);
		if (ret == false || bytes == 0)
		{
			//if send, recv then disconnect session

			int32 ErrCode = WSAGetLastError();
			std::cout << "[ERROR] GQCS Error: " << ErrCode << std::endl;
			continue;
		}
		ASSERT_CRASH(iocpEvent == NULL);

		switch (iocpEvent->GetType())
		{
		case EventType::ACCEPT:
		{
			SessionPtr _session = iocpEvent->sessionRef;
			_session->ProcessAccept();
			break;
		}
		case EventType::CONNECT:
		{
			SessionPtr _session = iocpEvent->sessionRef;
			_session->ProcessConnect();
			break;
		}
		case EventType::DISCONNECT:
		{
			SessionPtr _session = iocpEvent->sessionRef;
			_session->ProcessDisconnect();
			break;
		}
		case EventType::SEND:
		{
			SessionPtr _session = iocpEvent->sessionRef;
			_session->ProcessSend(bytes);
			break;
		}
		case EventType::RECV:
		{
			SessionPtr _session = iocpEvent->sessionRef;
			_session->ProcessRecv(bytes);
			break;
		}
		default:
			std::cout << "[FAIL] Wrong EventType: " << (uint8)iocpEvent->GetType() << std::endl;
			ASSERT_CRASH(false);
		}
	}
}
