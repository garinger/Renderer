#pragma once

#include <chrono>
#include <deque>

class Clock
{
public:
	Clock();

	// This is called after each main loop iteration
	void Tick();

	unsigned long long int GetFrameCount() const { return m_FrameCount; };
	double GetFrameTime() const { return m_FrameTime; };
	double GetFrameRate() const { return m_FrameRate; };
	double GetRecentFrameRate() const { return m_RecentFrameRate; };

private:
	unsigned long long int m_FrameCount;
	double m_FrameTime;
	double m_FrameRate;

	std::chrono::duration<double> m_Delta;

	// Time stamps used for calculating frame time
	std::chrono::time_point<std::chrono::steady_clock> m_Curr, m_Prev, m_LastRecentFrameRateUpdate;

	// Keep track of the last x frame rates so we can get a more stable average of frame rate
	double m_RecentFrameRate;
	// Using a deque instead of a regular queue because for whatever reason, you can't iterate through
	// a normal queue. Although, I'm using the deque like a standard queue.
	std::deque<double> m_RecentFrameRateSamples;
	int m_NumRecentFrameRateSamples;
	double m_MillisecondsBetweenRecentFrameRateUpdates;

	void CalculateRecentFrameRate();
};