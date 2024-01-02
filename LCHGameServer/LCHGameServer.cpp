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

    GIocpManager = new IocpManager(L"127.0.0.1", 8888, 3200);
    GIocpManager->Initialize();
    GIocpManager->BindAndListen();
    GIocpManager->StartWorker();
    GIocpManager->StartAccept<ClientSession>();

	GZoneManager.SpawnNpc();

    std::thread tickThread = std::thread([] 
	{
		Ticker ticker([]() { GZoneManager.TickUpdate(); },
		std::chrono::duration<int64, std::milli>(15));
	});


    while (true)
    {

    }

    GIocpManager->Join();
    tickThread.join();

    return true;
}
