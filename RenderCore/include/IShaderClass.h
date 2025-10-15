#pragma once
#include <cstdint>
#include <memory>

namespace gfx {

    class IShaderClass {
    public:
        virtual ~IShaderClass() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        // Matrices (for DX11 you need to use row-major)
        virtual void SetMatrices(const float* world4x4, const float* view4x4, const float* proj4x4) = 0;

        // Optional Pixel Shaders params
        virtual void SetColor(const float rgba[4]) = 0;        
        //virtual void SetTime(float t) = 0;                     
        //virtual void SetResolution(uint32_t w, uint32_t h) = 0;
    };
}