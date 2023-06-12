#pragma once
#include "IocpObject.h"
#include "IocpEvent.h"
#include "IocpCore.h"
#include "Session.h"
#include "SessionManager.h"

class Acceptor : public IocpObject
{
public:
	Acceptor(uint32 _maxSessionCnt, SessionFactory _onAcceptCompleted, Wstring _ip, uint16 _port);
	~Acceptor();

	bool StartAccept();
	bool PostAccept(AcceptEvent* _acceptEvent);
	IocpCorePtr GetIocpCore() { return iocpCore; }

	// Inherited via IocpObject
	virtual HANDLE GetHandle() override;
	virtual bool Dispatch(IocpEvent* _iocpEvent, int32 bytes = 0) override;

private:
	SOCKET acceptSocket = INVALID_SOCKET;
	std::vector<AcceptEvent*> acceptEvents;
	IocpCorePtr iocpCore;
	Wstring ip;
	uint16 port;
	SessionFactory onAcceptCompleted;
	uint32 maxSessionCnt;
};

