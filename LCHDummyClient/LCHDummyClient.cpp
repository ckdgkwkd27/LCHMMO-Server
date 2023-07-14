#include "pch.h"
#include "SocketUtil.h"
#include "SessionManager.h"
#include "ServerSession.h"
#include "IocpManager.h"
#include "ServerPacketHandler.h"

int main()
{
    ServerPacketHandler::Init();

    IocpManager* iocpManager = new IocpManager(L"127.0.0.1", 7777, 10);
    iocpManager->Initialize();
    iocpManager->StartConnect<ServerSession>();
    iocpManager->StartWorker();

    while (true)
    {

    }

    iocpManager->Join();

    return true;
}
