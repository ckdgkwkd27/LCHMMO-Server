#pragma once
#include "CircularBuffer.h"
#include "SocketUtil.h"
#include "IocpObject.h"

#define BUFFER_SIZE 65535

class IocpServer;

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session();
	~Session();

	SOCKET GetSocket() { return sessionSocket; }
	void SetConnected(bool cond) { isConnected = cond; }

	void Register();
	bool PostSend(CircularBuffer _sendBuffer);
	bool FlushSend();

	bool PreRecv();
	bool PostRecv();
	
	bool PostAccept();

	bool PostConnect();
	bool PostDisconnect();

	bool ProcessAccept();
	bool ProcessSend(int32 bytes);
	bool ProcessRecv(int32 bytes);
	bool ProcessConnect();
	bool ProcessDisconnect();

private:
	SOCKET sessionSocket = INVALID_SOCKET;
	AcceptEvent sessionAcceptEvent;
	ConnectEvent sessionConnectEvent;
	DisconnectEvent sessionDisconnectEvent;
	SendEvent sessionSendEvent;
	RecvEvent sessionRecvEvent;

	AtomicBool isConnected = false;
	RecursiveMutex sendLock;

public:
	Wstring ip;
	uint32 port;
	SOCKADDR_IN sockAddrIn;

	char AcceptBuffer[64] = { 0, };
	CircularBuffer sendBuffer;
	CircularBuffer recvBuffer;

public:
	virtual void OnConnected() {}
	virtual void OnDisconnected() {}
	virtual size_t OnSend(size_t len) { return len; }
	virtual void OnRecv(char* buffer, size_t len) {}
};

