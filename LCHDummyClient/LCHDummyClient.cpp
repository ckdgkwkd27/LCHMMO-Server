#include "pch.h"
#include "SessionManager.h"

int main()
{
	std::shared_ptr<Session> session = GSessionManager.CreateSession(iocpCore, factory);
	session->ip = L"127.0.0.1";
	session->port = 7777;
	if (session->PostConnect() == false)
		return false;
	
	std::vector<std::thread> workerThreads(2);
	for (uint32 i = 0; i < 2; i++)
	{
		workerThreads[i] = std::thread()
	}
}
