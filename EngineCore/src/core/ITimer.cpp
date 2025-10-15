#include "core/ITimer.h"
#include "platform/common/StdChrono_Timer.h"

/// ----------------------------------------------------------------
/// ITimer::Create
/// ----------------------------------------------------------------
/// Default simple static factory. Using std::chrono for now

std::unique_ptr<core::ITimer> core::ITimer::Create()
{
    auto timer = std::make_unique<core::ptm::StdChrono_Timer>();

    if (timer && timer->Initialize())
        return timer;

    return nullptr;
}