#include "core/Engine.h"
#include "core/Core_Macros.h"
#include "core/events/WindowEvent.h"
#include "core/InputSystem.h"
#include "core/Key_Defines.h"
#include <cassert>

/// ----------------------------------------------------------------
/// Engine Instance
/// ----------------------------------------------------------------

core::Engine* core::Engine::s_Engine = nullptr;

/// ----------------------------------------------------------------
/// Engine Ctor
/// ----------------------------------------------------------------

core::Engine::Engine() {

	assert(!s_Engine && "Application Already Exists");
	s_Engine = this;

	// Create Window
	WindowSpecs winSpecs;
	m_Window = IWindow::Create(winSpecs);
	assert(m_Window && "Failed to create window");

	// Register Callback
	m_Window->SetEventCallback(CORE_BIND_EVENT_FN(Engine::OnEvent));  // Maybe I can pass only OnEvent

	// Create Renderer 
	gfx::RendererDesc rendererDesc;
	rendererDesc.windowHandle = m_Window->GetNativeHandle();
	rendererDesc.width = winSpecs.width;
	rendererDesc.height = winSpecs.height;
	m_Renderer = gfx::IRenderer::Create(rendererDesc);
	assert(m_Renderer && "Failed to create renderer");

	// Create Timer
	// Initialize
	m_Timer = ITimer::Create();
}

/// ----------------------------------------------------------------
/// Engine Dtor
/// ----------------------------------------------------------------

core::Engine::~Engine() {

	Shutdown();
}

/// ----------------------------------------------------------------
/// Engine::Start
/// ----------------------------------------------------------------

void core::Engine::Start() {

	m_Running = true;
	Run();
}

/// ----------------------------------------------------------------
/// Engine::Close
/// ----------------------------------------------------------------

void core::Engine::Close() {

	m_Running = false;
}

/// ----------------------------------------------------------------
/// Engine::OnEvent
/// ----------------------------------------------------------------

void core::Engine::OnEvent(Event& event) {

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowCloseEvent>(CORE_BIND_EVENT_FN(Engine::OnWindowClose));
	// On WindowResize...

	// forward event to layers
	// the first one that handles the event breaks
	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {

		if (event.m_Handled)
			break;

		(*it)->OnEvent(event);
	}
}

/// ----------------------------------------------------------------
/// Engine::OnEvent
/// ----------------------------------------------------------------

void core::Engine::Run() {

	while (m_Running) {

		// Checks for Window Events 
		// Resolve immediately events here for the moment
		m_Window->PollEvents();

		// Check if user has pressed Escape Key
		if (InputSystem::GetInstance().IsKeyDown(KDEF_ESCAPE))
		{
			Close();
			break;
		}

		// Update Timer
		m_Timer->Tick();

		// Update Layers
		for (auto& layer : m_LayerStack)
		{
			layer->OnUpdate(m_Timer->GetDeltaTime());
		}

		// Render Layers
		for (auto& layer : m_LayerStack)  // if no modification, use const
		{
			layer->OnRender();
		}
	}
}

/// ----------------------------------------------------------------
/// Engine::Shutdown
/// ----------------------------------------------------------------

void core::Engine::Shutdown()
{
	// Since I'm using unique_ptr I don't need to manually delete stuff
	// Also Win32Window releases its resources in its dtor
	// Probably also the Renderer
}

/// ----------------------------------------------------------------
/// Engine::OnWindowClose
/// ----------------------------------------------------------------

bool core::Engine::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}