///
///
/// 

#pragma once

#include "core/events/Event.h"

namespace core {

    /// --------------------------------------------------------------
    /// MouseMovedEvent
    /// --------------------------------------------------------------

	class MouseMovedEvent : public Event {

    public:
        MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse)
    
    private:
        float m_MouseX{};
        float m_MouseY{};
	};

    /// --------------------------------------------------------------
    /// MouseButtonEvent
    /// --------------------------------------------------------------
    
    class MouseButtonEvent : public Event {

    public:

        int GetMouseButton() const { return m_Button; }
        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton)

    protected:

        MouseButtonEvent(int button, float x, float y) 
            : m_Button(button)
            , m_MouseX(x)
            , m_MouseY(y) 
        {}

        int m_Button;
        float m_MouseX;
        float m_MouseY;
    };

    /// --------------------------------------------------------------
    /// MouseButtonPressedEvent
    /// --------------------------------------------------------------
    
    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonPressedEvent(int button, float x, float y)
            : MouseButtonEvent(button, x, y) {
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    /// --------------------------------------------------------------
    /// MouseButtonReleasedEvent
    /// --------------------------------------------------------------
    
    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonReleasedEvent(int button, float x, float y)
            : MouseButtonEvent(button, x, y) {
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}