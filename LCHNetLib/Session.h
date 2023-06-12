#pragma once
#include "CircularBuffer.h"
#include "SocketUtil.h"
#include "IocpObject.h"

#define BUFFER_SIZE 65535

class IocpServer;

class Session : public IocpObject
{
public:
	Session();
	~Session();

	SOCKET GetSocket() { return sessionSocket; }
	
	bool PostSend(const char* buffer, size_t len);
	bool FlushSend();

	bool PreRecv();
	bool PostRecv();

	bool PostAccept();

	bool PostConnect();
	bool PostDisconnect();

	bool ProcessSend(int32 bytes);
	bool ProcessRecv(int32 bytes);
	bool ProcessConnect();
	bool ProcessDisconnect();

private:
	SOCKET sessionSocket = INVALID_SOCKET;
	ConnectEvent sessionConnectEvent;
	DisconnectEvent sessionDisconnectEvent;
	SendEvent sessionSendEvent;
	RecvEvent sessionRecvEvent;

	AtomicBool isConnected = false;
	RecursiveMutex sendLock;

public:
	Wstring ip;
	uint32 port;
	SOCKADDR_IN sockAddr;

	CircularBuffer sendBuffer;
	CircularBuffer recvBuffer;

public:
	HANDLE GetHandle() override;
	bool Dispatch(IocpEvent* _iocpEvent, int32 bytes) override;

	virtual void OnConnected() {}
	virtual void OnDisconnected() {}
	virtual size_t OnSend(size_t len) { return len; }
	virtual void OnRecv(char* buffer, size_t len);
};

