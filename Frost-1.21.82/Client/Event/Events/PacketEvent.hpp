#pragma once

class PacketEvent : public Event
{
public:
    PacketEvent(LoopbackPacketSender* sender, Packet* packet)
    {
        mSender = sender;
        mPacket = packet;
    }

    EventType getType() const override { return EventType::PacketSendToServer; }
    LoopbackPacketSender* mSender;
    Packet* mPacket;
};