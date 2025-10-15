#include "MDX11Renderer.h"
#include "DX11VertexBuffer.h"
#include "DX11IndexBuffer.h"
#include "DX11ShaderClass.h"
#include <cassert>
#include <sstream>
#include <vector>

/// ----------------------------------------------------------------
/// MDX11Renderer Ctor
/// ----------------------------------------------------------------

gfx::MDX11Renderer::MDX11Renderer(const RendererDesc& desc) : m_RenderDesc(desc)
{
	bool result = Initialize();
	assert(result);
}

/// ----------------------------------------------------------------
/// MDX11Renderer Dtor
/// ----------------------------------------------------------------

gfx::MDX11Renderer::~MDX11Renderer() {

	// Without this sometimes I have troubles
	if (m_SwapChain) {
		m_SwapChain->SetFullscreenState(FALSE, nullptr);
	}
}

/// ----------------------------------------------------------------
/// MDX11Renderer::BeginFrame
/// ----------------------------------------------------------------
/// It is called at the beginning of each frame. 
/// It initializes the buffers so they are blank and 
/// ready to be drawn to.

void gfx::MDX11Renderer::BeginFrame(float red, float green, float blue, float alpha) {

	const float color[4] = { red, green, blue, alpha };

	// Clear the back buffer to the specified color.
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);

	// Clear the depth buffer to 1.0f (maximum depth) and the stencil buffer to 0.
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);  // second param: D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL
}

/// ----------------------------------------------------------------
/// MDX11Renderer::Draw
/// ----------------------------------------------------------------

void gfx::MDX11Renderer::Draw(uint32_t indexCount)
{
	m_DeviceContext->DrawIndexed(indexCount, 0, 0);
}

/// ----------------------------------------------------------------
/// MDX11Renderer::PresentFrame
/// ----------------------------------------------------------------
/// It is called at the end of each frame. 
/// It presents the back buffer to the front buffer 
/// and displays the rendered scene on the screen.

void gfx::MDX11Renderer::PresentFrame()
{
	// Present the back buffer to the screen 
	// since rendering is complete.
	if (m_RenderDesc.vsync)
	{
		// If vsync is enabled, present the back buffer to 
		// the front buffer and lock the frame rate to the 
		// refresh rate of the monitor.
		m_SwapChain->Present(1, 0); // 1 means vsync is enabled
	}
	else
	{
		// If vsync is not enabled, present the back buffer 
		// to the front buffer without locking the frame rate.
		m_SwapChain->Present(0, 0); // 0 means vsync is disabled
	}
}

/// ----------------------------------------------------------------
/// MDX11Renderer::Resize
/// ----------------------------------------------------------------
/// not supported for now

void gfx::MDX11Renderer::Resize(uint32_t w, uint32_t h)
{
	m_DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	m_RenderTargetView.Reset();
	m_DepthStencilView.Reset();
	m_DepthStencilBuffer.Reset();

	m_SwapChain->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0);
	CreateRenderTargetView();
	CreateDepthStencilBuffer();
	SetupViewport();
}

/// ----------------------------------------------------------------
/// MDX11Renderer::CreateVertexBuffer
/// ----------------------------------------------------------------

std::shared_ptr<gfx::IVertexBuffer> gfx::MDX11Renderer::CreateVertexBuffer(
	const void* vertices, 
	uint32_t size, 
	uint32_t stride)
{
	auto ctx = GetRenderContext();
	return std::make_shared<DX11VertexBuffer>(ctx, vertices, size, stride);
}

/// ----------------------------------------------------------------
/// MDX11Renderer::CreateIndexBuffer
/// ----------------------------------------------------------------

std::shared_ptr<gfx::IIndexBuffer> gfx::MDX11Renderer::CreateIndexBuffer(
	const uint32_t* indices, 
	uint32_t count)
{
	auto ctx = GetRenderContext();
	return std::make_shared<DX11IndexBuffer>(ctx, indices, count);
}

/// ----------------------------------------------------------------
/// MDX11Renderer::CreateShader
/// ----------------------------------------------------------------

std::shared_ptr<gfx::IShaderClass> gfx::MDX11Renderer::CreateShader(
	const wchar_t* vsPath,
	const wchar_t* psPath)
{
	if (!vsPath || !psPath)
	{
		OutputDebugString(L"[MDX11Renderer] Invalid shader path(s)\n");
		return nullptr;
	}

	auto ctx = GetRenderContext();

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout =
	{
		{
			"POSITION",								// Semantic name
			0,										// Semantic index
			DXGI_FORMAT_R32G32B32_FLOAT,			// Format
			0,										// Input Slot
			0,										// AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,			// InputSlotClass
			0										// InstanceDataStepRate
		},
		{
			"NORMAL", 
			0, 
			DXGI_FORMAT_R32G32B32_FLOAT, 
			0,
			D3D11_APPEND_ALIGNED_ELEMENT, 
			D3D11_INPUT_PER_VERTEX_DATA, 
			0
		},
		{
			"TEXCOORD", 
			0, 
			DXGI_FORMAT_R32G32_FLOAT, 
			0,
			D3D11_APPEND_ALIGNED_ELEMENT, 
			D3D11_INPUT_PER_VERTEX_DATA, 
			0
		}
	};

	return std::make_shared<DX11ShaderClass>(ctx, vsPath, psPath, inputLayout);
}

/// ----------------------------------------------------------------
/// MDX11Renderer::GetVideoCardInfo
/// ----------------------------------------------------------------

void gfx::MDX11Renderer::GetVideoCardInfo(char* cardName, int& memory) const
{
	strcpy_s(cardName, 128, m_VideoCardDescription);
	memory = m_VideoCardMemory;
}

/// ----------------------------------------------------------------
/// MDX11Renderer::SetBackBufferRenderTarget
/// ----------------------------------------------------------------
/// useful in render to texture

void gfx::MDX11Renderer::SetBackBufferRenderTarget()
{
	// Bind the render target view to the output render pipeline.
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
}

/// ----------------------------------------------------------------
/// MDX11Renderer::ResetViewport
/// ----------------------------------------------------------------

void gfx::MDX11Renderer::ResetViewport()
{
	// Reset the viewport to the full size of the window.
	m_DeviceContext->RSSetViewports(1, &m_Viewport);
}

/// ----------------------------------------------------------------
/// MDX11Renderer::SetFullscreen
/// ----------------------------------------------------------------

void gfx::MDX11Renderer::SetFullscreen(bool enable)
{
	if (!m_SwapChain) return;
	if (m_IsFullscreen == enable) return;

	m_IsFullscreen = enable;
	m_SwapChain->SetFullscreenState(enable, nullptr);

	std::wstringstream ss;
	ss << L"[MDX11Renderer] Fullscreen " << (enable ? L"enabled" : L"disabled") << L"\n";
	OutputDebugString(ss.str().c_str());
}

/// ----------------------------------------------------------------
/// MDX11Renderer::ListAvailableAdapters
/// ----------------------------------------------------------------

/*
std::vector<std::wstring> gfx::MDX11Renderer::ListAvailableAdapters()
{
	std::vector<std::wstring> gpuNames{};
	Microsoft::WRL::ComPtr<IDXGIFactory1> factory;
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(factory.GetAddressOf())))) {
		OutputDebugString(L"[DXGI] Failed to create factory.\n");
		return gpuNames;
	}

	UINT i = 0;
	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	while (factory->EnumAdapters(i, adapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
		DXGI_ADAPTER_DESC desc;
		if (SUCCEEDED(adapter->GetDesc(&desc))) {
			std::wstringstream ss;
			ss << desc.Description << L" (VRAM: " << (desc.DedicatedVideoMemory / 1024 / 1024) << L" MB)";
			gpuNames.push_back(ss.str());
		}
		++i;
	}

	return gpuNames;
}
*/

/// ----------------------------------------------------------------
/// MDX11Renderer::GetRenderContext
/// ----------------------------------------------------------------

gfx::DX11RenderContext gfx::MDX11Renderer::GetRenderContext()
{
	DX11RenderContext ctx;
	ctx.Device = m_Device.Get();
	ctx.Context = m_DeviceContext.Get();

	return ctx;
}

/// ----------------------------------------------------------------
/// MDX11Renderer::Initialize
/// ----------------------------------------------------------------
/// The Initialize function is what does the entire 
/// setup of Direct3D for DirectX 11.

bool gfx::MDX11Renderer::Initialize() {

	// debug
	LogAdapters();

	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter{};
	
	if(!SelectAdapter(adapter)) return false;

	RefreshRateInfo rfInfo = GetRefreshRate(adapter);

	if (!InitializeDeviceAndSwapChain(adapter, rfInfo)) return false;
	if (!CreateRenderTargetView()) return false;
	if (!CreateDepthStencilBuffer()) return false;
	
	SetBackBufferRenderTarget();
	
	if (!CreateRasterizerState()) return false;

	SetupViewport();

	return true;
}

/// ----------------------------------------------------------------
/// MDX11Renderer::RetrieveAdapterInfo
/// ----------------------------------------------------------------

void gfx::MDX11Renderer::RetrieveAdapterInfo()
{

}

/// ----------------------------------------------------------------
/// MDX11Renderer::InitializeDeviceAndSwapChain
/// ----------------------------------------------------------------

bool gfx::MDX11Renderer::InitializeDeviceAndSwapChain(Microsoft::WRL::ComPtr<IDXGIAdapter>& adapter, const RefreshRateInfo& rfInfo) {

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_RenderDesc.width;
	swapChainDesc.BufferDesc.Height = m_RenderDesc.height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (m_RenderDesc.vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = rfInfo.Numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = rfInfo.Denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0; // 0 means no vsync
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1; // 1 means no vsync
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = static_cast<HWND>(m_RenderDesc.windowHandle);
	swapChainDesc.SampleDesc.Count = 1; // 1 sample per pixel
	swapChainDesc.SampleDesc.Quality = 0; // no multisampling

	if (m_RenderDesc.fullscreen)
	{
		swapChainDesc.Windowed = false; // full screen
	}
	else
	{
		swapChainDesc.Windowed = true; // windowed mode
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Setting the feature level so you can 
	// support multiple versions of Direct3D.
	D3D_FEATURE_LEVEL featureLevel{};
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		adapter.Get(),
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_SwapChain,
		&m_Device,
		nullptr,
		&m_DeviceContext)))
	{
		OutputDebugString(L"[MDX11Renderer] Failed to create device and swap chain.\n");
		return false;
	}

	return true;
}

/// ----------------------------------------------------------------
/// MDX11Renderer::CreateRenderTargetView
/// ----------------------------------------------------------------

bool gfx::MDX11Renderer::CreateRenderTargetView()
{
	// we need to get a pointer to the back buffer
	// and then attach it to the swap chain. 
	// We'll use the CreateRenderTargetView 
	// function to attach the back buffer to 
	// our swap chain.

	// Get the pointer of the back buffer from the swap chain.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer{};

	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()))))
	{
		OutputDebugString(L"Failed to get back buffer from swap chain.\n");
		return false;
	}

	// Create the render target view using the back buffer pointer.
	if (FAILED(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_RenderTargetView.GetAddressOf())))
	{
		OutputDebugString(L"Failed to create render target view.\n");
		return false;
	}

	// Set the immediate context to the device context
	// g_pImmediateContext = m_deviceContext;
	// g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL ); //numero di render target, puntatore back buffer, puntatore al depth stencil

	return true;
}

/// ----------------------------------------------------------------
/// MDX11Renderer::CreateDepthStencilBuffer
/// ----------------------------------------------------------------

bool gfx::MDX11Renderer::CreateDepthStencilBuffer()
{
	// to render properly 3D objects
	// At the same time, we will attach a stencil 
	// buffer to our depth buffer. 
	// The stencil buffer can be used to achieve 
	// effects such as motion blur, volumetric shadows, 
	// and other things.

	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	// Set up the description of the depth buffer.
	depthBufferDesc.Width = m_RenderDesc.width; // width of the depth buffer
	depthBufferDesc.Height = m_RenderDesc.height; // height of the depth buffer
	depthBufferDesc.MipLevels = 1; // number of mip map levels
	depthBufferDesc.ArraySize = 1; // number of textures in the array
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // format of the texture of the depth buffer
	depthBufferDesc.SampleDesc.Count = 1; // number of samples per pixel
	depthBufferDesc.SampleDesc.Quality = 0; // quality of the samples
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT; // usage of the depth buffer, only the gpu will use it
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // it tells that it will be used as a depth buffer
	depthBufferDesc.CPUAccessFlags = 0; // CPU access flags for the depth buffer
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	if (FAILED(m_Device->CreateTexture2D(&depthBufferDesc, nullptr, m_DepthStencilBuffer.GetAddressOf())))
	{
		OutputDebugString(L"Failed to create depth stencil buffer.\n");
		return false;
	}

	// === DEPTH STENCIL STATE ===
	// Now we need to setup the depth stencil 
	// description. 
	// This allows us to control what type of depth test 
	// Direct3D will do for each pixel
	// Initialize the description of the stencil state.

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state using the filled out description.
	if (FAILED(m_Device->CreateDepthStencilState(&depthStencilDesc, m_DepthStencilState.GetAddressOf())))
	{
		OutputDebugString(L"Failed to create depth stencil state.\n");
		return false;
	}

	// Now we use the device context to use the depth stencil state we just created.
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);

	// === DEPTH STENCIL VIEW ===
	// We need to create is the description of the 
	// view of the depth stencil buffer. 
	// We do this so that Direct3D knows to 
	// use the depth buffer as a depth stencil texture.
	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	if (FAILED(m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &depthStencilViewDesc, m_DepthStencilView.GetAddressOf())))
	{
		OutputDebugString(L"Failed to create depth stencil view.\n");
		return false;
	}

	return true;
}

/// ----------------------------------------------------------------
/// MDX11Renderer::CreateRasterizerState
/// ----------------------------------------------------------------

bool gfx::MDX11Renderer::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;  //D3D11_CULL_BACK
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	if (FAILED(m_Device->CreateRasterizerState(&rasterDesc, m_RasterState.GetAddressOf())))
	{
		OutputDebugString(L"Failed to create rasterizer state.\n");
		return false;
	}

	// Set the rasterizer state to the device context.
	m_DeviceContext->RSSetState(m_RasterState.Get());

	return true;
}

/// ----------------------------------------------------------------
/// MDX11Renderer::SetupViewport
/// ----------------------------------------------------------------

void gfx::MDX11Renderer::SetupViewport()
{
	m_Viewport.Width = static_cast<FLOAT>(m_RenderDesc.width);
	m_Viewport.Height = static_cast<FLOAT>(m_RenderDesc.height);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;

	m_DeviceContext->RSSetViewports(1, &m_Viewport);
}

/// ----------------------------------------------------------------
/// MDX11Renderer::SelectAdapter
/// ----------------------------------------------------------------

bool gfx::MDX11Renderer::SelectAdapter(Microsoft::WRL::ComPtr<IDXGIAdapter>& adapter)
{
	using namespace Microsoft::WRL;

	// Create a DirectX graphics interface factory.
	ComPtr<IDXGIFactory1> factory;
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(factory.GetAddressOf())))) {
		OutputDebugString(L"[DXGI] Failed to create factory.\n");
		return false;
	}

	// Use the factory to create an adapter for 
	// the primary graphics interface (video card).
	// 0 is the default adapter
	// I'll target for now the first adapter: NVIDIA gpu
	// this is only for testing on my machine, you should enumerate 
	// all adapters and let the user choose which one to use.

	std::vector<ComPtr<IDXGIAdapter>> adapters;
	UINT i = 0;
	ComPtr<IDXGIAdapter> tadapter;

	while (factory->EnumAdapters(i, tadapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
		adapters.push_back(tadapter);
		++i;
	}

	if (adapters.empty()) {

		adapter = nullptr;
		OutputDebugString(L"List of Adapters is Empty!\n");
		return false;
	}
	
	if (m_RenderDesc.adapterIndex >= static_cast<int>(adapters.size())) {

		// use default adapter
		m_RenderDesc.adapterIndex = 0;
		OutputDebugString(L"apadterIndex chosen is greater than available adapters!\n");
	}
	
	DXGI_ADAPTER_DESC desc;
	adapters[m_RenderDesc.adapterIndex]->GetDesc(&desc);

	m_VideoCardMemory = static_cast<int>(desc.DedicatedVideoMemory / 1024 / 1024);
	size_t converted = 0;
	wcstombs_s(&converted, m_VideoCardDescription, desc.Description, 128);

	std::wstringstream ss;
	ss << L"[MDX11Renderer] Selected Adapter: " << desc.Description << L" (" << m_VideoCardMemory << L" MB)\n";
	OutputDebugString(ss.str().c_str());

	adapter = adapters[m_RenderDesc.adapterIndex];
	return true;
}

/// ----------------------------------------------------------------
/// MDX11Renderer::GetRefreshRate
/// ----------------------------------------------------------------

gfx::RefreshRateInfo gfx::MDX11Renderer::GetRefreshRate(Microsoft::WRL::ComPtr<IDXGIAdapter>& adapter) const
{
	RefreshRateInfo info{};
	Microsoft::WRL::ComPtr<IDXGIOutput> output{};

	// Enumerate the primary adapter outputs (monitor)
	if (FAILED(adapter->EnumOutputs(0, output.GetAddressOf()))) {
		OutputDebugString(L"[MDX11Renderer] Failed to enumerate adapter outputs.\n");
		return info;
	}

	// Get the number of modes that fit the display format 
	// for the adapter output.
	// DXGI_FORMAT_R8G8B8A8_UNORM is a common format 
	// for color data.

	unsigned int numModes = 0;

	output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	std::vector<DXGI_MODE_DESC> modes(numModes);

	// Create a list to hold all the possible display 

	if (FAILED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, modes.data()))) {
		OutputDebugString(L"[MDX11Renderer] Failed to get display mode list.\n");
		return info;
	}

	// Now go through all the display modes and find the 
	// one that matches the screen width and height.
	// When a match is found store update numerator
	// and denominator

	for (const auto& mode : modes) {
		if (mode.Width == m_RenderDesc.width && mode.Height == m_RenderDesc.height) {
			info.Numerator = mode.RefreshRate.Numerator;
			info.Denominator = mode.RefreshRate.Denominator;
			break;
		}
	}

	return info;
}

/// ----------------------------------------------------------------
/// MDX11Renderer::GetRefreshRateHz
/// ----------------------------------------------------------------

float gfx::MDX11Renderer::GetRefreshRateHz(Microsoft::WRL::ComPtr<IDXGIAdapter>& adapter) const
{
	RefreshRateInfo info = GetRefreshRate(adapter);
	if (info.Denominator == 0) return 0.0f;
	return static_cast<float>(info.Numerator) / static_cast<float>(info.Denominator);
}

/// ----------------------------------------------------------------
/// MDX11Renderer::LogAdapters
/// ----------------------------------------------------------------

void gfx::MDX11Renderer::LogAdapters()
{
	using namespace Microsoft::WRL;

	ComPtr<IDXGIFactory1> factory;
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(factory.GetAddressOf())))) {
		OutputDebugString(L"[IDXGI] Failed to create factory.\n");
		return;
	}

	UINT i = 0;
	ComPtr<IDXGIAdapter> adapter;
	while (factory->EnumAdapters(i, adapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);
		std::wstringstream ss;
		ss << L"[MDX11Renderer] Adapter " << i << L": " << desc.Description << L" (VRAM: "
			<< (desc.DedicatedVideoMemory / 1024 / 1024) << L" MB)\n";
		OutputDebugString(ss.str().c_str());
		++i;
	}
}
