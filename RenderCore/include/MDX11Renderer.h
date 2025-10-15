#pragma once

/// Specify the libraries to link when using this object module.
/// 
/// "d3d11.lib": it contains all the functions for Direct3D 11 for setting up and 
///              drawing 3D graphics with DirectX 11.
/// "dxgi.lib" : it contains tools to interface with the hardware on 
///              the computer and obtain information about refresh rate, 
///              video card used...
/// 
/// "d3dcompiler.lib": it contains functions to compile shaders and 
///                    other DirectX 11 related tasks.
/// 
/// <d3d11.h> : it contains the Direct3D 11 API

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>       // #include <dxgi1_2.h>
#include <wrl/client.h>
#include "IRenderer.h"

namespace gfx {

	struct RefreshRateInfo 
	{
		UINT Numerator = 0;
		UINT Denominator = 1;
	};

	struct DX11RenderContext final : public IRenderContext 
	{
		ID3D11Device* Device = nullptr;
		ID3D11DeviceContext* Context = nullptr;
	};

	class MDX11Renderer final : public IRenderer {

	public:

		MDX11Renderer(const RendererDesc& desc);
		~MDX11Renderer();

		MDX11Renderer(const MDX11Renderer&) = delete;
		MDX11Renderer& operator=(const MDX11Renderer&) = delete;

		// IRenderer implementation
		void BeginFrame(float r, float g, float b, float a) override;
		void Draw(uint32_t indexCount) override;
		void PresentFrame() override;
		void Resize(uint32_t w, uint32_t h) override;

		//IRenderContext GetRenderContext() override;

		std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const void* vertices, uint32_t size, uint32_t stride) override;
		std::shared_ptr<IIndexBuffer> CreateIndexBuffer(const uint32_t* indices, uint32_t count) override;
		std::shared_ptr<IShaderClass> CreateShader(const wchar_t* vsPath, const wchar_t* psPath) override;

		void GetVideoCardInfo(char* cardName, int& memory) const;
		void SetBackBufferRenderTarget();
		void ResetViewport();

		void SetFullscreen(bool enable);
		inline bool IsFullscreen() const { return m_IsFullscreen; }

		//static std::vector<std::wstring> ListAvailableAdapters();

		inline ID3D11Device* GetDevice() const { return m_Device.Get(); }
		inline ID3D11DeviceContext* GetContext() const { return m_DeviceContext.Get(); }
		inline IDXGISwapChain* GetSwapChain() const { return m_SwapChain.Get(); }

	private:

		RendererDesc m_RenderDesc{};
		bool m_IsFullscreen = false;
		int m_VideoCardMemory = 0;
		char m_VideoCardDescription[128]{};

		Microsoft::WRL::ComPtr<IDXGISwapChain>           m_SwapChain{};
		Microsoft::WRL::ComPtr<ID3D11Device>             m_Device{};
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>      m_DeviceContext{};
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   m_RenderTargetView{};
		Microsoft::WRL::ComPtr<ID3D11Texture2D>          m_DepthStencilBuffer{};
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>  m_DepthStencilState{};
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>   m_DepthStencilView{};
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>    m_RasterState{};

		D3D11_VIEWPORT m_Viewport{};

		// Get context
		DX11RenderContext GetRenderContext();

		// initialization helpers
		bool Initialize();
		void RetrieveAdapterInfo();
		bool InitializeDeviceAndSwapChain(Microsoft::WRL::ComPtr<IDXGIAdapter>& adapter, const RefreshRateInfo& rfInfo);
		bool CreateRenderTargetView();
		bool CreateDepthStencilBuffer();
		bool CreateRasterizerState();
		void SetupViewport();

		bool SelectAdapter(Microsoft::WRL::ComPtr<IDXGIAdapter>& adapter);
		// TODO: store the refresh rate
		RefreshRateInfo GetRefreshRate(Microsoft::WRL::ComPtr<IDXGIAdapter>& adapter) const;
		float GetRefreshRateHz(Microsoft::WRL::ComPtr<IDXGIAdapter>& adapter) const;
		void LogAdapters();
	};
}