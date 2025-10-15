///
/// TODO: Check if PollEvents handle everything correctly
/// 

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include "core/IWindow.h"
#include "core/events/Event.h"

namespace core::ptm {

	class Win32Window : public IWindow {

	public:
		Win32Window(const WindowSpecs& specs);
		~Win32Window() override;

		Win32Window(const Win32Window&) = delete;
		Win32Window& operator=(const Win32Window&) = delete;

		void PollEvents() override;
		void* GetNativeHandle() const override { return m_Hwnd; }

		// Event Management
		void SetEventCallback(const EventCallbackFn& callback) override { m_EventCallback = callback; }

	private:
		WindowSpecs m_Specs{};
		HINSTANCE m_Hinstance{};
		HWND m_Hwnd{};

		EventCallbackFn m_EventCallback;

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		
		// to handle the windows system messages that will get sent 
		// to the application while it is running.
		LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		// helpers
		void ProcessMouseButton(int button, bool pressed, float x, float y);
	};
}