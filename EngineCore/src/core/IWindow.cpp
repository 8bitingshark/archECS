#include "core/IWindow.h"
#include "platform/win/Win32_Window.h"

/// ----------------------------------------------------------------
/// IWindow::Create
/// ----------------------------------------------------------------
/// Default simple static factory. Using Win32Window for now

std::unique_ptr<core::IWindow> core::IWindow::Create(const WindowSpecs& desc)
{
    auto window = std::make_unique<ptm::Win32Window>(desc);

    if (window)
        return window;

    return nullptr;
}