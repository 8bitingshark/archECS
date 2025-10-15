#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include "IShaderClass.h"
#include "MDX11Renderer.h"

namespace gfx {

	class DX11ShaderClass : public IShaderClass {

	public:

		// vsPath/psPath are UTF-16 (wide)
		DX11ShaderClass(const DX11RenderContext& ctx,
			const wchar_t* vsPath,
			const wchar_t* psPath,
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayout);

		~DX11ShaderClass() override = default;

		// bind/unbind to gpu
		void Bind() override;
		void Unbind() override;

		// update shader params
		void SetMatrices(const float* world4x4, const float* view4x4, const float* proj4x4) override;
		void SetColor(const float rgba[4]) override;

	private:

		// Vertex Shader Constant Buffers
		struct MatrixBufferType
		{
			DirectX::XMFLOAT4X4 world;
			DirectX::XMFLOAT4X4 view;
			DirectX::XMFLOAT4X4 projection;
		};

		// Pixel Shader Constant Buffers
		struct ColorBufferType
		{
			DirectX::XMFLOAT4 color;
		};

		// Render Context
		DX11RenderContext m_Ctx{};

		// Shaders
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_PS;

		// Input Layout
		Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_InputLayout;

		// Buffers
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VSMatrixCB;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_PSColorCB;
		MatrixBufferType m_Matrices{};
		ColorBufferType m_Color{};

		// Utility
		static Microsoft::WRL::ComPtr<ID3DBlob> CompileFromFile(const wchar_t* path,
			const char* entry,
			const char* target);

		// helpers
		void SetShaderParams();
		void CreateMatrixCBuffer();
		void CreatePixelCBuffer();
	};
}