#pragma once

class KeyboardEvent : public Event
{
public:
    KeyboardEvent(uint64_t* key, bool* held) 
    {
        mKey = key;
        mHeld = held;
    }

    EventType getType() const override { return EventType::Keyboard; }
    uint64_t* mKey;
    bool* mHeld;
};