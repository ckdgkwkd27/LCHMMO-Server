#include "pch.h"
#include "Acceptor.h"
#include "SessionManager.h"
#include "IocpServer.h"

int main()
{
    GSessionManager.PrepareSessions(5000);

    GIocpServer = new IocpServer(L"127.0.0.1", 7777, 1000);
    GIocpServer->Initialize();
    GIocpServer->StartAccept();
    GIocpServer->Run();

    GIocpServer->Join();

    return true;
}
