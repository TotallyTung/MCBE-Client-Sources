#pragma once

class UpdateEvent : public Event
{
public:
    UpdateEvent(MinecraftUIRenderContext* context)
    {
        Context = context;
    }

    EventType getType() const override { return EventType::Update; }
    MinecraftUIRenderContext* Context;
};