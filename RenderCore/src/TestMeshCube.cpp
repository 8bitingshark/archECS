#include "TestMeshCube.h"
#include "MDX11Renderer.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include <DirectXMath.h>

/// ----------------------------------------------------------------
/// TestMeshCube::Create
/// ----------------------------------------------------------------
/// Note: I create vertices and indices on the stack, since then
/// are copied to the GPU side and I don't need to modify or store
/// them for now.

bool gfx::TestMeshCube::Create(IRenderer* renderer)
{
    // Create points in a clockwise order
    // Otherwise not drawn for the back face culling
    // Always remember that the order in which 
    // you send your vertices to the GPU is very important.
    
    // define vertices
    const Vertex vertices[] =
    {
        // Front face
        {{-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0, 0, -1}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, -1}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1.0f, 1.0f}},

        // Back face
        {{-0.5f, -0.5f,  0.5f}, {0, 0, 1}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0, 0, 1}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0, 0, 1}, {1.0f, 0.0f}},

        // Left face
        {{-0.5f, -0.5f,  0.5f}, {-1, 0, 0}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1, 0, 0}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1, 0, 0}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {1.0f, 1.0f}},

        // Right face
        {{ 0.5f, -0.5f, -0.5f}, {1, 0, 0}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1, 0, 0}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1, 0, 0}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1, 0, 0}, {1.0f, 1.0f}},

        // Top face
        {{-0.5f,  0.5f, -0.5f}, {0, 1, 0}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0, 1, 0}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 1, 0}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 1, 0}, {1.0f, 1.0f}},

        // Bottom face
        {{-0.5f, -0.5f,  0.5f}, {0, -1, 0}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0, -1, 0}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0, -1, 0}, {0.0f, 1.0f}},
    };

    // define indices
    const uint32_t indices[] =
    {
        0,  1,  2,  0,  2,  3,      // Front
        4,  5,  6,  4,  6,  7,      // Back
        8,  9, 10,  8, 10, 11,      // Left
        12, 13, 14, 12, 14, 15,     // Right
        16, 17, 18, 16, 18, 19,     // Top
        20, 21, 22, 20, 22, 23      // Bottom
    };

    const uint32_t vertexCount = static_cast<uint32_t>(std::size(vertices));
    const uint32_t indexCount = static_cast<uint32_t>(std::size(indices));

    m_IndexCount = indexCount;

    // since it is a local array, sizeof(vertices) is right
    m_VertexBuffer = renderer->CreateVertexBuffer(vertices, sizeof(vertices), sizeof(Vertex));
    m_IndexBuffer = renderer->CreateIndexBuffer(indices, indexCount);

    return m_VertexBuffer && m_IndexBuffer;
}

/// ----------------------------------------------------------------
/// TestMeshCube::LoadBuffersOnGPU
/// ----------------------------------------------------------------

void gfx::TestMeshCube::LoadBuffersOnGPU() const
{
    // Note: for now after IndexBuffer binding, 
    // I also set the PrimitiveTopology
    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();
}

/// ----------------------------------------------------------------
/// TestMeshCube::GetIndexCount
/// ----------------------------------------------------------------

uint32_t gfx::TestMeshCube::GetIndexCount() const
{
    return m_IndexCount;
}
