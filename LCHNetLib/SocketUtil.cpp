#include "pch.h"
#include "SocketUtil.h"

bool SocketUtil::Init()
{
    WSADATA _wsaData;
    RETURN_FALSE_ON_FAIL(::WSAStartup(MAKEWORD(2, 2), &_wsaData));
    return true;
}

void SocketUtil::Clear()
{
    WSACleanup();
}

SOCKET SocketUtil::CreateSocket()
{
    return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

void SocketUtil::CloseSocket(SOCKET& _socket)
{
    if (_socket != INVALID_SOCKET)
        closesocket(_socket);
    _socket = INVALID_SOCKET;
}

bool SocketUtil::Bind(SOCKET _socket, Wstring ip, uint16 port)
{
    SOCKADDR_IN sockin;
    ZeroMemory(&sockin, sizeof(sockin));

    IN_ADDR address;
    ::InetPtonW(AF_INET, ip.c_str(), &address);

    sockin.sin_family = AF_INET;
    sockin.sin_addr = address;
    sockin.sin_port = port;

    return SOCKET_ERROR != ::bind(_socket, reinterpret_cast<const SOCKADDR*>(&sockin), sizeof(SOCKADDR_IN));
}

bool SocketUtil::BindAnyAddress(SOCKET _socket, uint16 port)
{
    SOCKADDR_IN sockin;
    sockin.sin_family = AF_INET;
    sockin.sin_addr.s_addr = ::htonl(INADDR_ANY);
    sockin.sin_port = ::htons(port);

    return SOCKET_ERROR != ::bind(_socket, reinterpret_cast<const SOCKADDR*>(&sockin), sizeof(sockin));
}

bool SocketUtil::Listen(SOCKET _socket, int32 backlog)
{
    return SOCKET_ERROR != ::listen(_socket, backlog);
}

bool SocketUtil::SetOptionLinger(SOCKET _socket)
{
    LINGER option;
    option.l_linger = 0;
    option.l_onoff = 0;
    return SOCKET_ERROR != setsockopt(_socket, SOL_SOCKET, SO_LINGER, 
        reinterpret_cast<char*>(&option), sizeof(option));
}

bool SocketUtil::SetOptionReuseAddr(SOCKET _socket, bool flag)
{
    return SOCKET_ERROR != setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, 
        reinterpret_cast<char*>(&flag), sizeof(flag));
}

bool SocketUtil::SetOptionNoDelay(SOCKET _socket, bool flag)
{
    return SOCKET_ERROR != setsockopt(_socket, SOL_SOCKET, TCP_NODELAY, 
        reinterpret_cast<char*>(&flag), sizeof(flag));
}

bool SocketUtil::SetOptionUpdateAcceptSocket(SOCKET _socket, SOCKET listenSocket)
{
    return SOCKET_ERROR != setsockopt(_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
        reinterpret_cast<char*>(&listenSocket), sizeof(listenSocket));
}

bool SocketUtil::SetOptionKeepAlive(SOCKET _socket, bool flag)
{
    return SOCKET_ERROR != setsockopt(_socket, SOL_SOCKET, SO_KEEPALIVE,
        reinterpret_cast<char*>(&flag), sizeof(flag));
}

SOCKET SocketUtil::CreateListenSocket(Wstring ip, uint16 port)
{
    SOCKET _listenSocket;

    Init();
    Bind(_listenSocket, ip, port);
    SetOptionKeepAlive(_listenSocket, true);
    SetOptionLinger(_listenSocket);
    SetOptionNoDelay(_listenSocket, true);
    SetOptionReuseAddr(_listenSocket, true);
    
    return _listenSocket;
}