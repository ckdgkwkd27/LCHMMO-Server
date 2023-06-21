#include "pch.h"
#include "Acceptor.h"
#include "SessionManager.h"
#include "IocpServer.h"

int main()
{
    IocpServer* iocpServer = new IocpServer(L"127.0.0.1", 7777, 10);
    iocpServer->Initialize();
    iocpServer->StartAccept();
    iocpServer->Run();

    while (true)
    {

    }

    iocpServer->Join();

    return true;
}
