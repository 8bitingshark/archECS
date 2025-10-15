/// Events in this implementation are blocking.
/// When an event occurs it immediately gets dispatched.
/// An Event bus pattern could be a better strategy, so
/// the events will be processed in a specific section
/// of the update stage in the gameplay loop

#pragma once

#include <string>
#include <functional>
#include <iostream>

namespace core {

    enum class EventType {
        None = 0,
        WindowClose, WindowResize,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved
    };

    /// EventCategory allows to apply multiple categories 
    /// to the event type
    
    enum EventCategory {
        None = 0,
        EventCategoryApplication = 1 << 0,    // 1 shifted by x places, 0 in this case
        EventCategoryInput = 1 << 1,
        EventCategoryKeyboard = 1 << 2,
        EventCategoryMouse = 1 << 3,
        EventCategoryMouseButton = 1 << 4
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                               virtual EventType GetEventType() const override { return GetStaticType(); }\
                               virtual const char* GetDebugName() const override { return #type; }  // convert identifier in string literal

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


    /// --------------------------------------------------------------
    /// Event
    /// --------------------------------------------------------------
    
    class Event {

    public:
        virtual ~Event() = default;

        /// We want to know if an event can be handled
        /// or not. When we dispatch events to various layers,
        /// we want be able to decide if we want handle or not

        bool m_Handled = false;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetDebugName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetDebugName(); }

        /// Allows to filter event's categories.
        /// It returns true if at least is in the given category.
        /// It might be in other categories.

        inline bool IsInCategory(EventCategory category) const { 
            return GetCategoryFlags() & category;
        }
    };

    /// --------------------------------------------------------------
    /// EventDispatcher
    /// --------------------------------------------------------------
    
    class EventDispatcher {

    public:
        EventDispatcher(Event& event) : m_Event(event) {}

        /// Somewhere in your engine you could implement a 
        /// function to handle the event, usually in the same class 
        /// you want to handle the event
        /// F will be deduced by the compiler
        
        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.m_Handled |= func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };

    // For a logging library
    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}