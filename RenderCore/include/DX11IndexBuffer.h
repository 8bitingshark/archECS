#pragma once

#include "IIndexBuffer.h"
#include "MDX11Renderer.h"
#include <d3d11.h>
#include <wrl/client.h>
// #include <cassert>

namespace gfx {

	class DX11IndexBuffer : public IIndexBuffer {

	public:
		DX11IndexBuffer(const DX11RenderContext& ctx, const uint32_t* indices, uint32_t count);
		void Bind() const override;
		uint32_t GetCount() const override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer{};
		DX11RenderContext m_RenderContext{};
		uint32_t m_Count{};
	};
}