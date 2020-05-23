#if PLATFORM_WINDOWS

#include "Core/Timing/GlobalTimer.h"

#include "Windows.h"

std::shared_ptr<CGlobalTimer> CGlobalTimer::GlobalTimerSingleton;

LARGE_INTEGER WindowsFrequency, WindowsStart, WindowsCurrent;

CGlobalTimer::CGlobalTimer()
{
	QueryPerformanceFrequency(&WindowsFrequency);
	CyclesPerSecond = WindowsFrequency.QuadPart;

	CyleLengthInSeconds = 1.0 / (double)CyclesPerSecond;

	QueryPerformanceCounter(&WindowsStart);
	StartTimeCycles = WindowsStart.QuadPart;
}

unsigned long long CGlobalTimer::GetCurrentCycles() const
{
	QueryPerformanceCounter(&WindowsCurrent);
	return WindowsCurrent.QuadPart;
}

#endif // PLATFORM_WINDOWS
