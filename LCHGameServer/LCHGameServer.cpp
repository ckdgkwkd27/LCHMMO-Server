#include "pch.h"
#include "Acceptor.h"

#include "IocpServer.h"

int main()
{
    std::shared_ptr<Acceptor> acceptor = std::make_shared<Acceptor>(5000, 
        std::make_shared<Session>(), L"127.0.0.1", 7777);
    acceptor->StartAccept();

    GIocpServer = new IocpServer();

    SYSTEM_INFO myInfo;
    GetSystemInfo(&myInfo);
    std::vector<std::thread> workerThreads(myInfo.dwNumberOfProcessors);
    for (uint32 i = 0; i < myInfo.dwNumberOfProcessors; i++)
    {
        workerThreads[i] = std::thread(acceptor->GetIocpCore()->Dispatch());
    }

    for (uint32 i = 0; i < myInfo.dwNumberOfProcessors; i++)
    {
        workerThreads[i].join();
    }

    return true;
}
