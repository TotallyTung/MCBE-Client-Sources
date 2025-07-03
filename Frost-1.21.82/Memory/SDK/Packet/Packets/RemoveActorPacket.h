#pragma once

class RemoveActorPacket : public Packet {
public:
    int64_t mRuntimeID; // this+0x30
};