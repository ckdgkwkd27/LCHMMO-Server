#include "pch.h"
#include "SocketUtil.h"

int main()
{
	SocketUtil::Init();
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addr = {};
	IN_ADDR address;
	::InetPtonW(AF_INET, L"127.0.0.1", &address);
	addr.sin_addr = address;
	addr.sin_family = AF_INET;
	addr.sin_port = 7777;

	connect(sock, (sockaddr*)&addr, sizeof(addr));

	std::cout << "Start Connect.." << std::endl;

	while (true)
	{

	}
}
