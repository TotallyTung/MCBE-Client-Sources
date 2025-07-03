#pragma once

class GetTimeofDayEvent : public Event
{
public:
    GetTimeofDayEvent(float original)
    {
        originalTime = original;
    }

    float originalTime;
    float* modifier;
    EventType getType() const override { return EventType::GetTimeofDay; }
};