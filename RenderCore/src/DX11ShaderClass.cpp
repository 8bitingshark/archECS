#include "DX11ShaderClass.h"

namespace gfx {

    inline DirectX::XMMATRIX LoadXMMatrixFromFloats(const float* m)
    {
        using namespace DirectX;
        return XMMATRIX(
            m[0], m[1], m[2], m[3],
            m[4], m[5], m[6], m[7],
            m[8], m[9], m[10], m[11],
            m[12], m[13], m[14], m[15]);
    }
}

/// ----------------------------------------------------------------
/// DX11ShaderClass Ctor
/// ----------------------------------------------------------------

gfx::DX11ShaderClass::DX11ShaderClass(
    const DX11RenderContext& ctx,
    const wchar_t* vsPath,
    const wchar_t* psPath,
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayout)
    : m_Ctx(ctx)
{
    assert(m_Ctx.Device && m_Ctx.Context);

    // Compile shaders
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = CompileFromFile(vsPath, "main", "vs_5_0");
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob = CompileFromFile(psPath, "main", "ps_5_0");

    assert(vsBlob && psBlob);

    // Create shader objects
    // Vertex shader obj
    HRESULT hr = m_Ctx.Device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        m_VS.GetAddressOf());

    assert(SUCCEEDED(hr));

    // Pixel shader obj
    hr = m_Ctx.Device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        m_PS.GetAddressOf());

    assert(SUCCEEDED(hr));

    // Create Input Layout
    hr = m_Ctx.Device->CreateInputLayout(
        inputLayout.data(),    // polygon layout of RT
        (UINT)inputLayout.size(),  // numElements of RT
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        m_InputLayout.GetAddressOf());
    assert(SUCCEEDED(hr));

    // Create constant buffers
    CreateMatrixCBuffer();
    CreatePixelCBuffer();

    // Default color
    m_Color.color = DirectX::XMFLOAT4(1, 1, 1, 1);
}

/// ----------------------------------------------------------------
/// DX11ShaderClass::Bind
/// ----------------------------------------------------------------

void gfx::DX11ShaderClass::Bind()
{
    SetShaderParams();

    m_Ctx.Context->IASetInputLayout(m_InputLayout.Get());
    m_Ctx.Context->VSSetShader(m_VS.Get(), nullptr, 0);
    m_Ctx.Context->PSSetShader(m_PS.Get(), nullptr, 0);
}

/// ----------------------------------------------------------------
/// DX11ShaderClass::Unbind
/// ----------------------------------------------------------------

void gfx::DX11ShaderClass::Unbind()
{
}

/// ----------------------------------------------------------------
/// DX11ShaderClass::SetMatrices
/// ----------------------------------------------------------------

void gfx::DX11ShaderClass::SetMatrices(const float* world4x4, const float* view4x4, const float* proj4x4)
{
    assert(world4x4 && view4x4 && proj4x4 && "Matrix pointers must be valid");
    assert(m_Ctx.Context && "Context must be valid");
    assert(m_VSMatrixCB && "Matrix constant buffer must be created");

    // store matrices
    DirectX::XMStoreFloat4x4(&m_Matrices.world, LoadXMMatrixFromFloats(world4x4));
    DirectX::XMStoreFloat4x4(&m_Matrices.view, LoadXMMatrixFromFloats(view4x4));
    DirectX::XMStoreFloat4x4(&m_Matrices.projection, LoadXMMatrixFromFloats(proj4x4));
}

/// ----------------------------------------------------------------
/// DX11ShaderClass::SetColor
/// ----------------------------------------------------------------

void gfx::DX11ShaderClass::SetColor(const float rgba[4])
{
    m_Color.color = DirectX::XMFLOAT4(rgba[0], rgba[1], rgba[2], rgba[3]);
}

/// ----------------------------------------------------------------
/// DX11ShaderClass::CompileFromFile
/// ----------------------------------------------------------------

Microsoft::WRL::ComPtr<ID3DBlob> gfx::DX11ShaderClass::CompileFromFile(
    const wchar_t* path,
    const char* entry,
    const char* target)
{
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG)
    flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob, errorBlob;

    HRESULT hr = D3DCompileFromFile(
        path,
        nullptr, nullptr,
        entry, target,
        flags, 0,
        shaderBlob.GetAddressOf(),
        errorBlob.GetAddressOf());

    if (FAILED(hr))
    {
        if (errorBlob)
            OutputDebugStringA((const char*)errorBlob->GetBufferPointer());
        assert(false && "Shader compilation failed");
    }

    return shaderBlob;
}

/// ----------------------------------------------------------------
/// DX11ShaderClass::SetShaderParams
/// ----------------------------------------------------------------

void gfx::DX11ShaderClass::SetShaderParams()
{
    // Update matrices

    D3D11_MAPPED_SUBRESOURCE mapped{};
    HRESULT hr = m_Ctx.Context->Map(m_VSMatrixCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    if (FAILED(hr))
    {
        OutputDebugString(L"[DX11ShaderClass] Failed to map matrix buffer.\n");
        return;
    }

    std::memcpy(mapped.pData, &m_Matrices, sizeof(MatrixBufferType));

    m_Ctx.Context->Unmap(m_VSMatrixCB.Get(), 0);
    m_Ctx.Context->VSSetConstantBuffers(0, 1, m_VSMatrixCB.GetAddressOf());


    // Update color

    D3D11_MAPPED_SUBRESOURCE cmapped{};
    if (SUCCEEDED(m_Ctx.Context->Map(m_PSColorCB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &cmapped)))
    {
        std::memcpy(cmapped.pData, &m_Color, sizeof(ColorBufferType));
        m_Ctx.Context->Unmap(m_PSColorCB.Get(), 0);
    }

    m_Ctx.Context->PSSetConstantBuffers(0, 1, m_PSColorCB.GetAddressOf());
}

/// ----------------------------------------------------------------
/// DX11ShaderClass::CreateMatrixCBuffer
/// ----------------------------------------------------------------

void gfx::DX11ShaderClass::CreateMatrixCBuffer()
{
    D3D11_BUFFER_DESC matrixBufferDesc{};
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // Usage type
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType); // Size of the buffer
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Bind flags
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU access flags
    matrixBufferDesc.MiscFlags = 0; // Misc flags
    matrixBufferDesc.StructureByteStride = 0; // Structure byte stride

    HRESULT hr = m_Ctx.Device->CreateBuffer(&matrixBufferDesc, nullptr, m_VSMatrixCB.GetAddressOf());
    assert(SUCCEEDED(hr));
}

/// ----------------------------------------------------------------
/// DX11ShaderClass::CreatePixelCBuffer
/// ----------------------------------------------------------------

void gfx::DX11ShaderClass::CreatePixelCBuffer()
{
    D3D11_BUFFER_DESC colorBufferDesc{};
    colorBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // Usage type
    colorBufferDesc.ByteWidth = sizeof(ColorBufferType); // Size of the buffer
    colorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Bind flags
    colorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU access flags
    colorBufferDesc.MiscFlags = 0; // Misc flags
    colorBufferDesc.StructureByteStride = 0; // Structure byte stride

    HRESULT hr = m_Ctx.Device->CreateBuffer(&colorBufferDesc, nullptr, m_PSColorCB.GetAddressOf());
    assert(SUCCEEDED(hr));
}
