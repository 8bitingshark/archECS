#include "DX11IndexBuffer.h"
#include <cassert>

gfx::DX11IndexBuffer::DX11IndexBuffer(const DX11RenderContext& ctx, const uint32_t* indices, uint32_t count)
{
	assert(ctx.Device && ctx.Context);
	assert(indices && count > 0);

	m_RenderContext = ctx;
	m_Count = count;

	D3D11_BUFFER_DESC indexBufferDesc{};

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(uint32_t) * count;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData{};

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	HRESULT hr = m_RenderContext.Device->CreateBuffer(&indexBufferDesc, &indexData, m_Buffer.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void gfx::DX11IndexBuffer::Bind() const
{
	assert(m_RenderContext.Context && "Device context is null in DX11IndexBuffer");
	m_RenderContext.Context->IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// for now I can put that here
	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	m_RenderContext.Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

uint32_t gfx::DX11IndexBuffer::GetCount() const
{
	return m_Count;
}
