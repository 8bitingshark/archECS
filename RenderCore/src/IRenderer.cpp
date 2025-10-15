#include "MDX11Renderer.h"

/// ----------------------------------------------------------------
/// IRenderer::Create
/// ----------------------------------------------------------------
/// Default simple static factory. Using MDX11Renderer for now

std::unique_ptr<gfx::IRenderer> gfx::IRenderer::Create(const RendererDesc& desc)
{
    auto renderer = std::make_unique<gfx::MDX11Renderer>(desc);

    if (renderer)
        return renderer;

    return nullptr;
}