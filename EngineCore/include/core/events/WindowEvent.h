///
///
/// 

#pragma once

#include "core/events/Event.h"
#include <sstream>

namespace core {

    /// --------------------------------------------------------------
    /// WindowCloseEvent
    /// --------------------------------------------------------------
    
    class WindowCloseEvent : public Event {
    public:
        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    /// --------------------------------------------------------------
    /// WindowResizeEvent
    /// --------------------------------------------------------------
    
    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int w, unsigned int h) : m_Width(w), m_Height(h) {}
        inline unsigned int GetWidth() const { return m_Width; }
        inline unsigned int GetHeight() const { return m_Height; }

        // just for debug, would be bad for performance
        std::string ToString() const override {

            std::stringstream ss;
            ss << "Window Resize Event: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

    private:
        unsigned int m_Width{};
        unsigned int m_Height{};
    };
}