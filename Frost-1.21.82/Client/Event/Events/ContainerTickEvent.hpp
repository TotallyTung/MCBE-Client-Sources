#pragma once

class ContainerTickEvent : public Event
{
public:
    ContainerTickEvent(ContainerScreenController* controller)
    {
        mController = controller;
    }

    EventType getType() const override { return EventType::ContainerTick; }
    ContainerScreenController* mController;
};