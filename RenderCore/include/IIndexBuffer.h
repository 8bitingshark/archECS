#pragma once
#include <memory>
#include <vector>

namespace gfx {

    class IIndexBuffer {
    public:
        virtual ~IIndexBuffer() = default;
        virtual void Bind() const = 0;
        virtual uint32_t GetCount() const = 0;
        //static std::shared_ptr<IIndexBuffer> Create(uint32_t* indices, uint32_t count);
    };
}