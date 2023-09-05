#include "pch.h"
#include "Acceptor.h"
#include "SessionManager.h"
#include "IocpManager.h"
#include "ClientSession.h"
#include "ClientPacketHandler.h"
#include "PlayerManager.h"
#include "ZoneManager.h"
#include "Ticker.h"

int main()
{
    GPlayerManager.Initialize();
    GZoneManager.Initialize();
    Ticker ticker([]() {std::cout << "Tick!" << std::endl; }, std::chrono::duration<int64, std::milli>(1000));
    ticker.Start();

    ClientPacketHandler::Init();

    IocpManager* iocpManager = new IocpManager(L"127.0.0.1", 8888, 20);
    iocpManager->Initialize();
    iocpManager->BindAndListen();
    iocpManager->StartWorker();
    iocpManager->StartAccept<ClientSession>();

    while (true)
    {

    }

    iocpManager->Join();

    return true;
}
