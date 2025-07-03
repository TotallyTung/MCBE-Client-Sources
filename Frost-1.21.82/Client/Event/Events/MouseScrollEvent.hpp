#pragma once

class MouseScrollEvent : public Event
{
public:
    MouseScrollEvent(bool direction) {
        mMouseDirection = direction;
    }

    EventType getType() const override { return EventType::MouseScroll; }
    bool mMouseDirection;
};