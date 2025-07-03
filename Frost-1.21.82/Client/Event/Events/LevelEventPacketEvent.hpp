#pragma once

class LevelEventPacketEvent : public Event
{
public:
    LevelEventPacketEvent(LevelEventPacket* pkt)
    {
        mPacket = pkt;
    }

    EventType getType() const override { return EventType::LevelEventPkt; }
    LevelEventPacket* mPacket;
};