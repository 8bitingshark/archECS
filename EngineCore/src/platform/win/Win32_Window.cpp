#include "platform/win/Win32_Window.h"
#include "core/InputSystem.h"
#include "core/events/KeyEvent.h"
#include "core/events/MouseEvent.h"
#include "core/events/WindowEvent.h"
#include "core/Key_Defines.h"
#include <cassert>
#include <windowsx.h>

/// ----------------------------------------------------------------
/// WindowInstance
/// ----------------------------------------------------------------
/// global instance for this translation unit
/// needed for WndProc

static core::ptm::Win32Window* s_WindowInstance = nullptr;

/// ----------------------------------------------------------------
/// Win32Window Ctor
/// ----------------------------------------------------------------

core::ptm::Win32Window::Win32Window(const WindowSpecs& specs) : m_Specs(specs)
{
    // register this instance
    assert(s_WindowInstance == nullptr && "Only one Win32Window can exist");
    s_WindowInstance = this;

    bool result{};

    // define the window class attributes
    WNDCLASSEX wc{ 0 };
    // define the screen settings for full screen mode
    DEVMODE dmScreenSettings{ 0 };
    int posX{}, posY{}; // Position of the window

    // get the instance of this application
    m_Hinstance = GetModuleHandle(nullptr);

    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_Hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hCursor = nullptr;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = m_Specs.Title.c_str();
    wc.hIconSm = nullptr;

    // register window class
    RegisterClassEx(&wc);

    // Determine the resolution of the clients desktop screen.
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    // Setup the screen settings depending 
    // on whether it is running in full screen or in windowed mode.
    if (m_Specs.fullscreen)
    {
        // If full screen set the screen to maximum size of the users desktop and 32bit.
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = (unsigned long)width;
        dmScreenSettings.dmPelsHeight = (unsigned long)height;
        dmScreenSettings.dmBitsPerPel = 32;  // Set the bit depth to 32 bits per pixel. (4 channels of 8 bits each)
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Change the display settings to full screen.
        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

        // Set the position of the window to the top left corner.
        posX = posY = 0;
    }
    else
    {
        // windowed mode
        // Place the window in the middle of the screen.
        posX = (GetSystemMetrics(SM_CXSCREEN) - m_Specs.width) / 2;
        posY = (GetSystemMetrics(SM_CYSCREEN) - m_Specs.height) / 2;
    }

    // Create the window with the defined attributes
    m_Hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        m_Specs.Title.c_str(), // name of the window class
        m_Specs.Title.c_str(), // title of the window
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,  // | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU
        posX,
        posY,
        m_Specs.width,
        m_Specs.height,
        nullptr, // no parent window
        nullptr, // no menu
        m_Hinstance, // instance handle
        nullptr
    );

    // Bring the window up on the screen and set it as main focus.
    ShowWindow(m_Hwnd, SW_SHOW);
    SetForegroundWindow(m_Hwnd);
    SetFocus(m_Hwnd);

    // Hide the mouse cursor.
    // ShowCursor(false);
}

// Init
/*
    m_Width = desc.Width;
    m_Height = desc.Height;
    m_Title = std::wstring(desc.Title.begin(), desc.Title.end());
    m_HInstance = GetModuleHandle(nullptr);
    s_WindowInstance = this;

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = m_HInstance;
    wc.lpszClassName = m_Title.c_str();
    RegisterClassW(&wc);

    m_Hwnd = CreateWindowExW(
        0, m_Title.c_str(), m_Title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        m_Width, m_Height,
        nullptr, nullptr, m_HInstance, nullptr
    );

    ShowWindow(m_Hwnd, SW_SHOW);
    return true;
*/


/// ----------------------------------------------------------------
/// Win32Window Dtor
/// ----------------------------------------------------------------

core::ptm::Win32Window::~Win32Window()
{
    OutputDebugString(L"Win32Window Dtor called");

    if (m_Specs.fullscreen)
    {
        ChangeDisplaySettings(nullptr, 0);
    }

    if (m_Hwnd)
    {
        DestroyWindow(m_Hwnd);
        m_Hwnd = nullptr;
    }
        
    if (!m_Specs.Title.empty())
    {
        UnregisterClass(m_Specs.Title.c_str(), m_Hinstance);
        m_Hinstance = nullptr;
    }
       
    // release pointer
    s_WindowInstance = nullptr;
}

/// ----------------------------------------------------------------
/// Win32Window::PollEvents
/// ----------------------------------------------------------------

void core::ptm::Win32Window::PollEvents()
{
    MSG msg{};

    // handle the windows messages
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // No more handling WM_QUIT here
}

/// ----------------------------------------------------------------
/// Win32Window::WndProc
/// ----------------------------------------------------------------

LRESULT CALLBACK core::ptm::Win32Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (s_WindowInstance) {
        return s_WindowInstance->HandleMessage(hwnd, msg, wParam, lParam);
    }
        
    return DefWindowProc(hwnd, msg, wParam, lParam);  // or DefWindowProcW
}

/// ----------------------------------------------------------------
/// Win32Window::HandleMessage
/// ----------------------------------------------------------------

LRESULT core::ptm::Win32Window::HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        //===Window===
        case WM_CLOSE:
        case WM_QUIT:
        case WM_DESTROY:
        {
            OutputDebugString(L"WM_CLOSE/QUIT/DESTROY");

            // Event
            WindowCloseEvent e;
            if (m_EventCallback)
            {
                m_EventCallback(e);
            }

            PostQuitMessage(0);
            return 0;
        }

        //===Keyboard===
        case WM_KEYDOWN:
        {
            OutputDebugString(L"WM_KEYDOWN");

            int key = static_cast<int>(wParam);

            // Update Input
            InputSystem::GetInstance().OnKeyDown(key);

            // Check bit 30 to know if it the first
            // time for that key to be pressed or not
            bool isRepeat = (lParam & (1 << 30)) != 0;

            // Event
            KeyPressedEvent e(key, isRepeat);
            if (m_EventCallback)
            {
                m_EventCallback(e);
            }

            return 0;
        }

        case WM_KEYUP: 
        {
            OutputDebugString(L"WM_KEYUP");

            int key = static_cast<int>(wParam);

            // Update input
            InputSystem::GetInstance().OnKeyUp(key);

            // Event
            KeyReleasedEvent e(key);
            if (m_EventCallback)
            {
                m_EventCallback(e);
            }

            return 0;
        }

        //===Mouse===
        case WM_MOUSEMOVE:
        {
            OutputDebugString(L"WM_MOUSEMOVE");

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            float xPos = static_cast<float>(x);
            float yPos = static_cast<float>(y);

            // Input
            InputSystem::GetInstance().OnMouseMoved(xPos, yPos);

            // Event
            MouseMovedEvent e(xPos, yPos);
            if (m_EventCallback)
            {
                m_EventCallback(e);
            }

            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            OutputDebugString(L"WM_LBUTTONDOWN");

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            float xPos = static_cast<float>(x);
            float yPos = static_cast<float>(y);

            ProcessMouseButton(KDEF_LBUTTON, true, xPos, yPos);

            return 0;
        }

        case WM_LBUTTONUP:
        {
            OutputDebugString(L"WM_LBUTTONUP");

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            float xPos = static_cast<float>(x);
            float yPos = static_cast<float>(y);

            ProcessMouseButton(KDEF_LBUTTON, false, xPos, yPos);

            return 0;
        }

        case WM_MBUTTONDOWN:
        {
            OutputDebugString(L"WM_MBUTTONDOWN");

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            float xPos = static_cast<float>(x);
            float yPos = static_cast<float>(y);

            ProcessMouseButton(KDEF_MBUTTON, true, xPos, yPos);

            return 0;
        }

        case WM_MBUTTONUP:
        {
            OutputDebugString(L"WM_MBUTTONUP");

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            float xPos = static_cast<float>(x);
            float yPos = static_cast<float>(y);

            ProcessMouseButton(KDEF_MBUTTON, false, xPos, yPos);

            return 0;
        }

        case WM_RBUTTONDOWN:
        {
            OutputDebugString(L"WM_RBUTTONDOWN");

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            float xPos = static_cast<float>(x);
            float yPos = static_cast<float>(y);

            ProcessMouseButton(KDEF_RBUTTON, true, xPos, yPos);

            return 0;
        }

        case WM_RBUTTONUP:
        {
            OutputDebugString(L"WM_RBUTTONUP");

            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            float xPos = static_cast<float>(x);
            float yPos = static_cast<float>(y);

            ProcessMouseButton(KDEF_RBUTTON, false, xPos, yPos);

            return 0;
        }

        //===Default===
        // Pass all other messages to the default handler
        default:
        {
            return DefWindowProc(hwnd, msg, wParam, lParam); // Default message handling
        }
    }
}

/// ----------------------------------------------------------------
/// Win32Window::ProcessMouseButton
/// ----------------------------------------------------------------

void core::ptm::Win32Window::ProcessMouseButton(int button, bool pressed, float x, float y)
{
    if (pressed)
    {
        InputSystem::GetInstance().OnMouseButtonDown(button);
        MouseButtonPressedEvent e(button, x, y);
        if (m_EventCallback)
            m_EventCallback(e);
    }
    else
    {
        InputSystem::GetInstance().OnMouseButtonUp(button);
        MouseButtonReleasedEvent e(button, x, y);
        if (m_EventCallback)
            m_EventCallback(e);
    }
}
