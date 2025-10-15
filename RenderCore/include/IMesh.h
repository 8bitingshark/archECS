#pragma once
#include "IRenderer.h"

namespace gfx {

    class IMesh {
    public:
        virtual ~IMesh() = default;
        virtual bool Create(IRenderer* renderer) = 0;
        virtual void LoadBuffersOnGPU() const = 0;
        virtual uint32_t GetIndexCount() const = 0;
    };

}