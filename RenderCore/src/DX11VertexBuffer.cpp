#include "DX11VertexBuffer.h"
#include <cassert>

/// ----------------------------------------------------------------
/// DX11VertexBuffer Ctor
/// ----------------------------------------------------------------

gfx::DX11VertexBuffer::DX11VertexBuffer(const DX11RenderContext& ctx, const void* vertices, uint32_t size, uint32_t stride)
	: m_Stride(stride)
{
    assert(ctx.Context && ctx.Device);
    assert(vertices && size > 0 && stride > 0);

    m_RenderContext = ctx;
    m_Stride = stride; // Basically sizeof(Vertex), the size of one Vertex
    m_Offset = 0; // No offset

    
    D3D11_BUFFER_DESC desc{};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = size;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data{};
    data.pSysMem = vertices;
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    HRESULT hr = m_RenderContext.Device->CreateBuffer(&desc, &data, m_Buffer.GetAddressOf());
    assert(SUCCEEDED(hr));
}

/// ----------------------------------------------------------------
/// DX11VertexBuffer::Bind
/// ----------------------------------------------------------------

void gfx::DX11VertexBuffer::Bind() const
{
    assert(m_RenderContext.Context && "Device context is null in DX11VertexBuffer");

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    m_RenderContext.Context->IASetVertexBuffers(0, 1, m_Buffer.GetAddressOf(), &m_Stride, &m_Offset);
}
