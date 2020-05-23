#pragma once

#include <memory>

class CGlobalTimer
{
private:
	// StartTime is a snapshot time of when the timer is created, so instatiate at the start of app
	CGlobalTimer();

public:
	static inline std::shared_ptr<CGlobalTimer> Get()
	{
		return GlobalTimerSingleton ? GlobalTimerSingleton : (GlobalTimerSingleton = std::shared_ptr<CGlobalTimer>(new CGlobalTimer()));
	}

	inline unsigned long long GetFrameTimeCycles() const
	{
		return FrameTimeCycles;
	}

	inline double GetFrameTime() const
	{
		return FrameTime;
	}

	unsigned long long GetCurrentCycles() const;

	// TODO: make these private
	// Sets frame time
	inline void Tick()
	{
		const unsigned long long ThisFrameEndCycle = GetCurrentCycles();
		FrameTimeCycles = ThisFrameEndCycle - LastFrameCycle;
		FrameTime = ToTime(FrameTimeCycles);
		LastFrameCycle = ThisFrameEndCycle;
	}
	// TODO: end

	inline unsigned long long ToCyles(const double TimeSeconds)
	{
		return CyclesPerSecond * TimeSeconds;
	}

	inline double ToTime(const unsigned long long NumberOfCyles)
	{
		return CyleLengthInSeconds * NumberOfCyles;
	}

private:
	static std::shared_ptr<CGlobalTimer> GlobalTimerSingleton;

	unsigned long long StartTimeCycles = 0;

	// Total time the last frame took in cycles and seconds
	unsigned long long FrameTimeCycles = 0;
	double FrameTime = 0.0;

	// The cycle that the last frame ended on
	unsigned long long LastFrameCycle = 0;

	// Conversions
	unsigned long long CyclesPerSecond = 0;
	double CyleLengthInSeconds = 0.0;
};
