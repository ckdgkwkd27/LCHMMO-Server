#include "pch.h"
#include "IocpCore.h"

IocpCore::IocpCore()
{
    _iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
}

IocpCore::~IocpCore()
{
    CloseHandle(_iocpHandle);
}

HANDLE IocpCore::GetHandle()
{
    return _iocpHandle;
}

bool IocpCore::Register(IocpObjectPtr iocpObject)
{
    return CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, 0, 0);
}

bool IocpCore::Dispatch(uint32 timeout)
{
    DWORD bytes = 0;
    ULONG_PTR key;
    IocpEvent* iocpEvent = nullptr;

    if (GetQueuedCompletionStatus(_iocpHandle, &bytes, &key, reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeout))
    {
        IocpObjectPtr iocpObject = iocpEvent->owner;
        iocpObject->Dispatch(iocpEvent, bytes);
    }

    else
    {
        int32 ErrCode = WSAGetLastError();
        switch (ErrCode)
        {
        case WAIT_TIMEOUT:
            return false;
            break;
        default:
            IocpObjectPtr iocpObject = iocpEvent->owner;
            iocpObject->Dispatch(iocpEvent, bytes);
            break;
        }
    }

    return true;
}
