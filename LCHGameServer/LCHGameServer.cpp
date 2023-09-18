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

    ClientPacketHandler::Init();

    IocpManager* iocpManager = new IocpManager(L"127.0.0.1", 8888, 20);
    iocpManager->Initialize();
    iocpManager->BindAndListen();
    iocpManager->StartWorker();
    iocpManager->StartAccept<ClientSession>();

	GZoneManager.SpawnNpc();

    std::thread tickThread = std::thread([] 
	{
		Ticker ticker([]() { GZoneManager.TickUpdate(); },
		std::chrono::duration<int64, std::milli>(15));
	});


    while (true)
    {

    }

    iocpManager->Join();
    tickThread.join();

    return true;
}
