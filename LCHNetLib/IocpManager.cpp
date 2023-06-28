#include "pch.h"
#include "IocpManager.h"

char AcceptBuf[64] = { 0, };

IocpManager::IocpManager(std::wstring _ip, uint16 _port, uint32 _maxConnectionCnt)
{
	ip = _ip;
	port = _port;
	maxConnectionCnt = _maxConnectionCnt;
}

void IocpManager::Initialize()
{
	workerThreadsCnt = std::thread::hardware_concurrency() / 2;
	workerThreads.resize(workerThreadsCnt);

	iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(iocpHandle != NULL);

	listenSocket = SocketUtil::CreateListenSocket();
	HANDLE _handle = CreateIoCompletionPort((HANDLE)listenSocket, iocpHandle, 0, 0);
	if (_handle != iocpHandle)
	{
		ASSERT_CRASH(false);
	}

	std::cout << "[INFO] Initialization Success.." << std::endl;
	return;
}

void IocpManager::BindAndListen()
{
	SocketUtil::Bind(listenSocket, ip, port);
	SocketUtil::Listen(listenSocket);
	std::cout << "[INFO] Server Listen Start.." << std::endl;
}

bool IocpManager::StartWorker()
{
	for (uint32 i = 0; i < workerThreadsCnt; i++)
	{
		workerThreads[i] = std::thread(&IocpManager::WorkerThreadFunc, this);
	}

	return true;
}

bool IocpManager::Join()
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

void IocpManager::AcceptThreadFunc()
{
	while (GSessionManager.AcceptClientSession(maxConnectionCnt))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void IocpManager::WorkerThreadFunc()
{
	while (true)
	{
		DWORD bytes;
		ULONG_PTR key;
		IocpEvent* iocpEvent = nullptr;
		bool ret = GetQueuedCompletionStatus(iocpHandle, &bytes, &key, reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), INFINITE);
		ASSERT_CRASH(iocpEvent != NULL);

		if (ret == false)
		{
			int32 errCode = WSAGetLastError();
			switch (errCode)
			{
			case WAIT_TIMEOUT:
				return;
			case ERROR_NETNAME_DELETED:
				break;
			default:
				std::cout << "[FAIL] GQCS ErrorCode: " << WSAGetLastError() << std::endl;
				break;
			}
		}

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
