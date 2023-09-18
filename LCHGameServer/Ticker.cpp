#include "pch.h"
#include "Ticker.h"

Ticker::Ticker(OnTickFunction _onTickFunction, TickIntervalType _tickInterval)
	:onTickFunction(_onTickFunction),
	tickInterval(_tickInterval),
	isRunning(false)
{
	Start();
}

void Ticker::Start()
{
	if (isRunning) return;
	isRunning = true;
	Loop();
}

void Ticker::Stop()
{
	isRunning = false;
}

void Ticker::SetDuration(TickIntervalType _tickInterval)
{
	LockGuard guard(tickLock);
	tickInterval = _tickInterval;
}

void Ticker::Loop()
{
	while (isRunning)
	{
		//std::thread tickThread(onTickFunction);
		//tickThread.join();

		//LockGuard guard(tickLock);
		onTickFunction();
		TickIntervalType _tickInterval = tickInterval;
		std::this_thread::sleep_for(tickInterval);
	}
}
