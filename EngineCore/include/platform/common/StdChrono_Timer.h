/// ----------------------------------------------------------------
/// Class to handle time passed during simulation and deltaTime
/// using std::chrono
/// ----------------------------------------------------------------

#pragma once

#include "core\ITimer.h"
#include <chrono>

namespace core::ptm {

	class StdChrono_Timer : public ITimer {

	public:
		StdChrono_Timer() = default;
		StdChrono_Timer(const StdChrono_Timer&) = delete;

		bool Initialize() override;
		void Tick() override;

		float GetDeltaTime() const override { return m_deltaTime; }
		float GetTotalTime() const override { return m_totalTime; }

	private:
		std::chrono::high_resolution_clock::time_point m_start{};
		std::chrono::high_resolution_clock::time_point m_last{};
		float m_deltaTime{};
		float m_totalTime{};
	};
}