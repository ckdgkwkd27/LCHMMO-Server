#pragma once
#include "CircularBuffer.h"
#include "SocketUtil.h"
#include "IocpObject.h"

#define BUFFER_SIZE 65535

class IocpManager;
class SessionManager;

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session();
	~Session();

	SOCKET GetSocket() { return sessionSocket; }
	void SetConnected(bool cond) { isConnected = cond; }
	void SetListenSocket(SOCKET _listenSocket) { listenSocket = _listenSocket; }
	void SetIocpHandle(HANDLE _iocpHandle) { iocpHandle = _iocpHandle; }

	void Register();
	bool PostSend(CircularBufferPtr _sendBuffer);

	bool PostRecv();
	bool PostAccept();
	bool PostConnect(Wstring ip, uint32 port);
	bool PostDisconnect();

	bool ProcessAccept();
	bool ProcessSend(int32 bytes);
	bool ProcessRecv(int32 bytes);
	bool ProcessConnect();
	bool ProcessDisconnect();

private:
	SOCKET sessionSocket = INVALID_SOCKET;
	SOCKET listenSocket = INVALID_SOCKET;
	HANDLE iocpHandle = INVALID_HANDLE_VALUE;
	AcceptEvent sessionAcceptEvent;
	ConnectEvent sessionConnectEvent;
	DisconnectEvent sessionDisconnectEvent;
	SendEvent sessionSendEvent;
	RecvEvent sessionRecvEvent;

	AtomicBool isConnected = false;
	RecursiveMutex sendLock;

public:
	SOCKADDR_IN sockAddrIn;
	char AcceptBuffer[64] = { 0, };
	CircularBuffer sendBuffer;
	CircularBuffer recvBuffer;

public:
	virtual void OnAccepted() {}
	virtual void OnConnected() {}
	virtual void OnDisconnected() {}
	virtual void OnSend(uint32 len) { len = 0; }
	virtual uint32 OnRecv(char* buffer, uint32 len) { buffer = nullptr; return len; }
};

class PacketHeader
{
public:
	uint16 size;
	uint16 id;
};