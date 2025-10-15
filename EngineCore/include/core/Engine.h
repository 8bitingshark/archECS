///
///
/// 

#pragma once

#include <memory>
#include "core\IWindow.h"
#include "core\ITimer.h"
#include "core\LayerStack.h"
#include "IRenderer.h"

namespace core {

	class WindowCloseEvent;
	class KeyPressedEvent;

	class Engine {

	public:
		Engine();
		~Engine();

		/** Add a Layer to the Stack */
		template<typename TLayer, typename... Args>
			requires(std::is_base_of_v<Layer, TLayer>)
		void AddLayerToStack(Args&&... args) {
			m_LayerStack.PushLayer<TLayer>(std::forward<Args>(args)...);
		}

		void Start();
		void Close();

		/** Handles events */
		void OnEvent(Event& e);

		/** Returns the pointer to the Engine */
		static Engine& GetInstance() { return *s_Engine; }

		/** */
		gfx::IRenderer* GetRenderer() { return m_Renderer.get(); }

		/** Returns the reference to the Window */
		IWindow& GetWindow() { return *m_Window; }

	private:

		static Engine* s_Engine;

		bool m_Running = false;

		LayerStack m_LayerStack{};
		std::unique_ptr<IWindow> m_Window{};     // for now handling one window at a time
		std::unique_ptr<gfx::IRenderer> m_Renderer{};
		std::unique_ptr<ITimer> m_Timer{};

		void Run();
		void Shutdown();

		bool OnWindowClose(WindowCloseEvent& e);
	};

}