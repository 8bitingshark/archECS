///
/// Alternative way to construct and initialize an Engine
/// with concrete Window, Renderer and Timer.
/// 

#pragma once

#include <memory>
#include "Engine.h"
#include "platform\common\StdChrono_Timer.h"

namespace core {

	// Supported APIs
	enum class WindowAPI { Win32, GLFW };
	enum class RendererAPI { DX11, OpenGL };

	/// 
	/// EngineSpecs
	/// 
	
	struct EngineSpecs {

		WindowAPI winAPI = WindowAPI::Win32;
		RendererAPI rendererAPI = RendererAPI::DX11;

		std::string name = "archECS";
		unsigned int width = 1280;
		unsigned int height = 720;
	};

	///
	/// EngineFactory 
	///
	
	class EngineFactory {

	public:

		// helper for a default Engine
		static std::unique_ptr<Engine> CreateDefaultEngine() {
			EngineSpecs spec{};
			return CreateEngine(spec);
		}

		static std::unique_ptr<Engine> CreateEngine(const EngineSpecs& specs) {

			std::unique_ptr<IWindow> window;
			std::unique_ptr<gfx::IRenderer> renderer;
			std::unique_ptr<ITimer> timer = std::make_unique<ptm::StdChrono_Timer>();
		}
	};
}