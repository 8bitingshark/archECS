/// ----------------------------------------------------------------
/// Class to handle time passed during simulation and deltaTime
/// specifically for Windows platforms
/// ----------------------------------------------------------------

#pragma once

#include "core\ITimer.h"
#include <Windows.h>

namespace core::ptm {

	class Win_Timer : public ITimer {

    public:
        Win_Timer() = default;
        Win_Timer(const Win_Timer&) = delete;

        bool Initialize() override;
        void Tick() override;

        float GetDeltaTime() const override { return m_deltaTime; }
        float GetTotalTime() const override { return m_totalTime; }

    private:
        INT64 m_ticksPerSeconds{};
        float m_ticksPerMs{};

        INT64 m_startTime{};
        INT64 m_frameTime{};
        float m_deltaTime{};
        float m_totalTime{};
	};
}