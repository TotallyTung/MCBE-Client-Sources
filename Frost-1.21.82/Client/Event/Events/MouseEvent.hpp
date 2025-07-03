#pragma once

class MouseEvent : public Event
{
public:
    MouseEvent(__int8* key, bool* held) 
    {
        mKey = key;
        mHeld = held;
    }

    EventType getType() const override { return EventType::Mouse; }
    __int8* mKey;
    bool* mHeld;
};