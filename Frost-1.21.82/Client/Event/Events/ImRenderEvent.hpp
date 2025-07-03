#pragma once

class ImRenderEvent : public Event
{
public:
    ImRenderEvent() {}

    EventType getType() const override { return EventType::ImGuiRender; }
};
