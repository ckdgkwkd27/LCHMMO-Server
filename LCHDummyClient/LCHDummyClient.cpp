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

	SocketUtil::SetOptionNoDelay(sock, true);

	connect(sock, (sockaddr*)&addr, sizeof(addr));

	std::cout << "Start Connect.." << std::endl;

	send(sock, "Hello World", strlen("Hello World"), 0);
	send(sock, "South Korea", strlen("South Korea"), 0);
	send(sock, "¾È³çÇÏ¼¼¿ä~", strlen("¾È³çÇÏ¼¼¿ä~"), 0);

	char buffer[512] = { 0, };
	while (true)
	{
		if (recv(sock, buffer, 512, 0) != -1)
		{
			std::cout << buffer << std::endl;
		}
		memset(buffer, 0, sizeof(buffer));
	}
}
