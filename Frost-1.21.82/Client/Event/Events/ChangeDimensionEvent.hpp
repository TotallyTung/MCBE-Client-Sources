#pragma once

class DimensionEvent : public Event
{
public:
    DimensionEvent() {}

    EventType getType() const override { return EventType::ChangeDimension; }
};