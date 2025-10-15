#include "platform\common\StdChrono_Timer.h"

/// ----------------------------------------------------------------
/// StdChrono_Timer::Initialize
/// ----------------------------------------------------------------

bool core::ptm::StdChrono_Timer::Initialize()
{
    m_start = std::chrono::high_resolution_clock::now();
    m_last = m_start;

    return true;
}

/// ----------------------------------------------------------------
/// StdChrono_Timer::Tick
/// ----------------------------------------------------------------

void core::ptm::StdChrono_Timer::Tick()
{
    auto now = std::chrono::high_resolution_clock::now();
    m_deltaTime = std::chrono::duration<float>(now - m_last).count();
    m_totalTime += m_deltaTime;
    m_last = now;
}