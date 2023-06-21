#include "pch.h"
#include "SocketUtil.h"
#include "SessionManager.h"

int main()
{
	SocketUtil::Init();
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SessionManager sm;
	sm.PrepareSessions(10, INVALID_SOCKET, INVALID_HANDLE_VALUE);
	for (auto& ss : sm.GetSessionPool())
	{
		sockaddr_in addr = {};
		IN_ADDR address;
		::InetPtonW(AF_INET, L"127.0.0.1", &address);
		addr.sin_addr = address;
		addr.sin_family = AF_INET;
		addr.sin_port = 7777;

		ss->sockAddrIn = addr;
		connect(ss->GetSocket(), (sockaddr*)&addr, sizeof(addr));
	}

	SocketUtil::SetOptionNoDelay(sock, true);

	std::cout << "Start Connect All.." << std::endl;

	char buffer[512] = { 0, };
	while (true)
	{
		std::this_thread::sleep_for(0.5s);
		for (auto& ss : sm.GetSessionPool())
		{
			if (SOCKET_ERROR == send(ss->GetSocket(), "Look At Me~", strlen("Look At Me~"), 0))
				std::cout << "send Wrong~~~: " << WSAGetLastError() << std::endl;
		}

		for (auto& ss : sm.GetSessionPool())
		{
			if (SOCKET_ERROR == recv(ss->GetSocket(), buffer, 512, 0))
				std::cout << "recv Wrong~~~: " << WSAGetLastError() << std::endl;
			std::cout << buffer << std::endl;
			memset(buffer, 0, sizeof(buffer));
		}
	}
}
