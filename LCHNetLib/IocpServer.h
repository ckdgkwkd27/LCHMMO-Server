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
	
	void StartAccept();
	void WorkerThreadFunc();

	static char AcceptBuf[64] = { 0, };

private:
	std::wstring ip;
	uint16 port;
	uint32 maxConnectionCnt;
	SOCKET listenSocket;
	HANDLE iocpHandle;
	uint32 workerThreadsCnt;
	std::thread acceptThread;
	std::vector<std::thread> workerThreads;

public:
	SOCKET GetListenSocket() 
	{
		return listenSocket;
	}

};

extern IocpServer* GIocpServer;

