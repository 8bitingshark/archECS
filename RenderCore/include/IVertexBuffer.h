#pragma once
#include <memory>
#include <vector>

namespace gfx {

    class IVertexBuffer{
    public:
        virtual ~IVertexBuffer() = default;
        virtual void Bind() const = 0;
        //static std::shared_ptr<IVertexBuffer> Create(void* vertices, uint32_t size, uint32_t stride);
    };
}