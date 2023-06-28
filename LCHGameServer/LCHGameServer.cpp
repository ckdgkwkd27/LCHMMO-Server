#include "pch.h"
#include "Acceptor.h"
#include "SessionManager.h"
#include "IocpManager.h"
#include "ClientSession.h"

int main()
{
    IocpManager* iocpManager = new IocpManager(L"127.0.0.1", 7777, 10);
    iocpManager->Initialize();
    iocpManager->BindAndListen();
    iocpManager->StartAccept<ClientSession>();
    iocpManager->StartWorker();

    while (true)
    {

    }

    iocpManager->Join();

    return true;
}
