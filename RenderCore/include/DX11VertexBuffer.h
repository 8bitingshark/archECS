#pragma once

#include "IVertexBuffer.h"
#include "MDX11Renderer.h"
#include <d3d11.h>
#include <wrl/client.h>
// #include <cassert>

namespace gfx {

	class DX11VertexBuffer : public IVertexBuffer {

	public:
		DX11VertexBuffer(const DX11RenderContext& ctx, const void* vertices, uint32_t size, uint32_t stride);
		void Bind() const override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer{};
		DX11RenderContext m_RenderContext{};
		uint32_t m_Stride = 0;
		uint32_t m_Offset = 0;
	};
}