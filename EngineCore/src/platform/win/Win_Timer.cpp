#include "platform\win\Win_Timer.h"

/// ----------------------------------------------------------------
/// Win_Timer::Initialize
/// ----------------------------------------------------------------

bool core::ptm::Win_Timer::Initialize()
{
	// Get the frequency of the high-resolution performance counter (ticks per second).
	// If the function fails (e.g., unsupported hardware), return false.

	LARGE_INTEGER freq;
	if (!QueryPerformanceFrequency(&freq))
	{
		OutputDebugString(L"Failed to query performance frequency");
		return false;
	}

	// Store the frequency (number of ticks per second).
	m_ticksPerSeconds = freq.QuadPart;

	// Calculate how many milliseconds each tick represents.
	// 1000 milliseconds / ticks per second = milliseconds per tick.
	// This will be used to convert tick differences to milliseconds.
	m_ticksPerMs = 1000.0f / static_cast<float>(m_ticksPerSeconds);

	// Get the current value of the high-resolution performance counter.
	LARGE_INTEGER start;
	if (!QueryPerformanceCounter(&start))
	{
		OutputDebugString(L"Failed to retrieve the high-res performance counter");
		return false;
	}

	// Store the initial start time (in ticks).
	m_startTime = start.QuadPart;

	// Initialize the frame time to the start time (first frame).
	m_frameTime = m_startTime;

	// Initialize delta time (time between frames) and total elapsed time to zero.
	m_deltaTime = 0.0f;
	m_totalTime = 0.0f;

	// Initialization successful.
	return true;
}

/// ----------------------------------------------------------------
/// Win_Timer::Tick
/// ----------------------------------------------------------------

void core::ptm::Win_Timer::Tick()
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	INT64 currentTime = current.QuadPart;
	// currentTime - m_frameTime gives the number of ticks since the last frame
	// m_frequency is the number of ticks per second evaluated after the query
	m_deltaTime = static_cast<float>(currentTime - m_frameTime) / static_cast<float>(m_ticksPerSeconds);
	m_totalTime = static_cast<float>(currentTime - m_startTime) / static_cast<float>(m_ticksPerSeconds);
	m_frameTime = currentTime;
}