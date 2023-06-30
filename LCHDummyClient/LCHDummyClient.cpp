#include "pch.h"
#include "SocketUtil.h"
#include "SessionManager.h"
#include "ServerSession.h"
#include "IocpManager.h"

int main()
{
    IocpManager* iocpManager = new IocpManager(L"127.0.0.1", 7777, 100);
    iocpManager->Initialize();
    iocpManager->StartConnect<ServerSession>();
    iocpManager->StartWorker();

    while (true)
    {

    }

    iocpManager->Join();

    return true;
}
