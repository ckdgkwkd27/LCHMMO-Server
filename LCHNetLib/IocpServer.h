#pragma once
#include "SocketUtil.h"
#include "SessionManager.h"
#include "IocpEvent.h"

class IocpServer
{
public:
	IocpServer(std::wstring _ip, uint16 _port, uint32 _maxConnectionCnt);
	
	void Initialize();
	bool Run();
	bool Join();
	
	template <typename T>
	void StartAccept();

	void AcceptThreadFunc();
	void WorkerThreadFunc();

private:
	std::wstring ip;
	uint16 port;
	uint32 maxConnectionCnt;
	SOCKET listenSocket;
	HANDLE iocpHandle;
	uint32 workerThreadsCnt;
	std::thread AcceptThread;
	std::vector<std::thread> workerThreads;

public:
	HANDLE GetIocpHandle()
	{
		return iocpHandle;
	}

	SOCKET GetListenSocket() 
	{
		return listenSocket;
	}

	uint32 GetMaxConnectionCnt()
	{
		return maxConnectionCnt;
	}
};

template <typename T>
void IocpServer::StartAccept()
{
	GSessionManager.PrepareSessions<T>(maxConnectionCnt, listenSocket, iocpHandle);

	AcceptThread = std::thread(&IocpServer::AcceptThreadFunc, this);
	std::cout << "[INFO] Start Accept..." << std::endl;

}