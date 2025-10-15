
/// ITimer
/// Interface for timer implementation.
/// For now uses a simple factory method
/// to create the actual timer.
/// 

#pragma once

#include <memory>

namespace core {

	class ITimer {

	public:
		virtual ~ITimer() = default;

		virtual bool Initialize() = 0;
		virtual void Tick() = 0;

		virtual float GetDeltaTime() const = 0;
		virtual float GetTotalTime() const = 0;

		// simple static factory
		static std::unique_ptr<ITimer> Create();
	};
}