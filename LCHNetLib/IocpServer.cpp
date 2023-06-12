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
	SYSTEM_INFO sysInfo;
	DWORD workerThreadCnt = sysInfo.dwNumberOfProcessors;
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
		workerThreads[i] = std::thread(WorkerThreadFunc);
	}

	return true;
}

bool IocpServer::Join()
{
	if (acceptThread.joinable())
	{
		acceptThread.join();
	}

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
	while (true)
	{
		SessionPtr _session = GSessionManager.IssueSession(); //여기서 CreateIoCompPort 로 등록까지 시킨다
		SOCKET sessionSocket = _session->GetSocket();
		for (uint32 i = 0; i < maxConnectionCnt; i++)
		{
			AcceptEvent* acceptEvent = new AcceptEvent();
			acceptEvent->sessionRef = _session;
			acceptEvent->Init();

			DWORD bytesReceived = 0;

			if (false == AcceptEx(listenSocket, _session->GetSocket(),
				_session->recvBuffer.WritePos(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
				&bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
			{
				const INT32 errCode = WSAGetLastError();
				if (errCode != WSA_IO_PENDING) {
					std::cout << "[FAIL] Accept ErrorCode: " << errCode << std::endl;
					continue;
				}
			}
		}

	}
	while (GSessionManager.AcceptClientSession());
}

void IocpServer::WorkerThreadFunc()
{

}
