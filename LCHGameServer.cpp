#include "pch.h"
#include "Acceptor.h"
#include "SessionManager.h"
#include "IocpServer.h"

int main()
{
    GIocpServer = new IocpServer(L"127.0.0.1", 7777, 5);
    GIocpServer->Initialize();
    GIocpServer->StartAccept();
    GIocpServer->Run();

    while (true)
    {

    }

    GIocpServer->Join();

    return true;
}
