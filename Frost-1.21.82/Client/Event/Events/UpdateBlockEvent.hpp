#pragma once

class UpdateBlockEvent : public Event
{
public:
    UpdateBlockEvent(UpdateBlockPacket* pkt)
    {
        mPacket = pkt;
    }

    EventType getType() const override { return EventType::UpdateBlock; }
    UpdateBlockPacket* mPacket;
};