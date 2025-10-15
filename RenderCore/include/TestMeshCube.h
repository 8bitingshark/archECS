#pragma once
#include "IMesh.h"
#include <memory>
#include <directxmath.h>

namespace gfx {

    class TestMeshCube : public IMesh {

    public:

        bool Create(IRenderer* renderer) override;
        void LoadBuffersOnGPU() const override;
        uint32_t GetIndexCount() const override;

    private:

        struct Vertex {
            DirectX::XMFLOAT3 position;
            DirectX::XMFLOAT3 normal;
            DirectX::XMFLOAT2 uv;
        };

        std::shared_ptr<IVertexBuffer> m_VertexBuffer;
        std::shared_ptr<IIndexBuffer> m_IndexBuffer;
        uint32_t m_IndexCount = 0;
    };
}