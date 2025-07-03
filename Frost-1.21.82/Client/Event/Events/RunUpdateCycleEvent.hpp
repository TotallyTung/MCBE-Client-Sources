#pragma once

class RunUpdateCycleEvent : public Event
{
public:
    RunUpdateCycleEvent() {}
    
    EventType getType() const override { return EventType::RunUpdateCycle; }
};