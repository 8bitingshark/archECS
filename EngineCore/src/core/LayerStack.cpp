#include "core/LayerStack.h"

/// ----------------------------------------------------------------
/// LayerStack Dtor
/// ----------------------------------------------------------------

core::LayerStack::~LayerStack()
{
    for (auto& layer : m_Layers)
    {
        layer->OnDetach();
    }
}

/// ----------------------------------------------------------------
/// LayerStack::PopLayer
/// ----------------------------------------------------------------

void core::LayerStack::PopLayer()
{
    if (!m_Layers.empty()) {
        m_Layers.back()->OnDetach();
        m_Layers.pop_back();
    }
}
