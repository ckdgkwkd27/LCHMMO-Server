#include "pch.h"
#include "Acceptor.h"
#include "SessionManager.h"
#include "IocpServer.h"
#include "ClientSession.h"

int main()
{
    IocpServer* iocpServer = new IocpServer(L"127.0.0.1", 7777, 10);
    iocpServer->Initialize();
    iocpServer->StartAccept<ClientSession>();
    iocpServer->Run();

    while (true)
    {

    }

    iocpServer->Join();

    return true;
}
