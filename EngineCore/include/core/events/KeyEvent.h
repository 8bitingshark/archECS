///
///
/// 

#pragma once

#include "core/events/Event.h"
#include <sstream>

namespace core {

    /// --------------------------------------------------------------
    /// KeyEvent
    /// --------------------------------------------------------------
    
    class KeyEvent : public Event {
    public:
        inline int GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        explicit KeyEvent(int keycode) : m_KeyCode(keycode) {}
        int m_KeyCode;
    };

    /// --------------------------------------------------------------
    /// KeyPressedEvent
    /// --------------------------------------------------------------
    /// Handle the first pression event 
    /// and the repeat ones. (It depends from the OS/drivers underneath)
    
    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int keycode, bool repeat = false)
            : KeyEvent(keycode), m_Repeat(repeat) {
        }

        inline bool IsRepeat() const { return m_Repeat; }

        // debug
        std::string ToString() const override {

            std::stringstream ss;
            ss << "KeyPressedEvent: kCode: " << m_KeyCode << ", repeat: " << m_Repeat;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        bool m_Repeat;
    };

    /// --------------------------------------------------------------
    /// KeyReleasedEvent
    /// --------------------------------------------------------------
    
    class KeyReleasedEvent : public KeyEvent {
    public:
        explicit KeyReleasedEvent(int keycode)
            : KeyEvent(keycode) {
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };
}