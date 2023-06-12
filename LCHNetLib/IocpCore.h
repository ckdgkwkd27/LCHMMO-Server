#pragma once
#include "IocpObject.h"

class IocpCore
{
public:
	IocpCore();
	~IocpCore();
	
	HANDLE GetHandle();

	bool Register(IocpObjectPtr iocpObject);
	bool Dispatch(uint32 timeout = INFINITE);

private:
	HANDLE _iocpHandle = INVALID_HANDLE_VALUE;
};
