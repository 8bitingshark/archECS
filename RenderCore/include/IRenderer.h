#pragma once

#include <cstdint>
#include <memory>

namespace gfx {

    class IVertexBuffer;
    class IIndexBuffer;
    class IShaderClass;

	struct RendererDesc {

        void* windowHandle = nullptr;  // from IWindow::GetNativeHandle()
        uint32_t width = 1280; 
        uint32_t height = 720;
        float nearPlane = 0.3f;
        float farPlane = 1000.0f;
        int adapterIndex = 0;
        bool vsync = true;
        bool fullscreen = false;
        bool IsResizeable = false;
	};

    class IRenderContext {
    public:
        virtual ~IRenderContext() = default;
    };

    class IRenderer {
    public:
        virtual ~IRenderer() = default;
        virtual void BeginFrame(float r, float g, float b, float a) = 0;
        virtual void Draw(uint32_t indexCount) = 0;
        virtual void PresentFrame() = 0; // Present
        virtual void Resize(uint32_t w, uint32_t h) = 0;

        // simple static factory method
        static std::unique_ptr<IRenderer> Create(const RendererDesc& specs);

        //virtual IRenderContext GetRenderContext() = 0;

        // simple factory methods
        /*
        Usage examples:
        auto vb = m_Renderer->CreateVertexBuffer(vertices, size, stride);
        auto shader = m_Renderer->CreateShader(L"VS.hlsl", L"PS.hlsl");
        */
        virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const void* vertices, uint32_t size, uint32_t stride) = 0;
        virtual std::shared_ptr<IIndexBuffer> CreateIndexBuffer(const uint32_t* indices, uint32_t count) = 0;
        virtual std::shared_ptr<IShaderClass> CreateShader(const wchar_t* vsPath, const wchar_t* psPath) = 0;
    };

}
