#pragma once

class ActorEventPacket : public Packet {
public:
    int64_t        mRuntimeId; // this+0x30
    ActorEvent     mEventId;   // this+0x38
    int            mData;      // this+0x3c
};