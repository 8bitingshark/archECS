#pragma once

#include <cstdint> // uint32_t
#include <string>
#include <memory>
#include <functional>
#include "events/Event.h"

namespace core {

	struct WindowSpecs {

		std::wstring Title = L"archECS";
		uint32_t width = 1280;
		uint32_t height = 720;
		bool IsResizeable = false;
		bool fullscreen = false;
	};

	class IWindow {
	
	public:

		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~IWindow() = default;
		virtual void PollEvents() = 0;
		virtual void* GetNativeHandle() const = 0;  // HWND* (Win32) o GLFWwindow*
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0; // To handle Events

		// simple static factory method
		static std::unique_ptr<IWindow> Create(const WindowSpecs& specs);
	};

}