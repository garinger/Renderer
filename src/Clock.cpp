#include "Clock.h"

Clock::Clock()
	: m_FrameCount(0), m_FrameTime(0.0), m_FrameRate(0.0), m_Delta(0.0f),
	m_Curr(std::chrono::high_resolution_clock::now()), m_Prev(),
	m_LastRecentFrameRateUpdate(std::chrono::high_resolution_clock::now()),
	m_RecentFrameRate(0.0), m_RecentFrameRateSamples(), m_NumRecentFrameRateSamples(250),
	m_MillisecondsBetweenRecentFrameRateUpdates(200)
{

}

void Clock::Tick()
{
	// Keep track of the total frame rendered
	m_FrameCount++;

	// Set previous time to the last frame's current time
	m_Prev = m_Curr;

	// Get the new current time
	m_Curr = std::chrono::high_resolution_clock::now();

	// Get the time between the last frame and now
	m_Delta = m_Curr - m_Prev;

	// Calcualte the frame time in milliseconds
	m_FrameTime = m_Delta.count() * 1000.0;

	// Calcualte the frame rate
	m_FrameRate = 1000.0 / m_FrameTime;

	// Add the current frame rate sample to the queue
	m_RecentFrameRateSamples.push_back(m_FrameRate);

	// If the queue is too full, pop from the front of the queue
	if (m_RecentFrameRateSamples.size() > m_NumRecentFrameRateSamples)
	{
		m_RecentFrameRateSamples.pop_front();
	}

	// Update m_RecentFrameRate if it's been a certain amount of time since the last update
	std::chrono::duration<double> timeSinceLastUpdate = m_Curr - m_LastRecentFrameRateUpdate;
	if (timeSinceLastUpdate.count() * 1000.0 > m_MillisecondsBetweenRecentFrameRateUpdates)
	{
		CalculateRecentFrameRate();
		m_LastRecentFrameRateUpdate = std::chrono::high_resolution_clock::now();
	}
}

void Clock::CalculateRecentFrameRate()
{
	// Iterate through the queue and sum up the frame rates
	double sum = 0.0;
	for (const double i : m_RecentFrameRateSamples)
	{
		sum += i;
	}
	
	// Get the average frame rate
	m_RecentFrameRate = sum / m_RecentFrameRateSamples.size();
}