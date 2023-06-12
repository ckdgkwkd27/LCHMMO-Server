#pragma once
#include "IocpEvent.h"

class IocpObject : public std::enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() = 0;
	virtual bool Dispatch(IocpEvent* _iocpEvent, int32 bytes = 0) = 0;
};

