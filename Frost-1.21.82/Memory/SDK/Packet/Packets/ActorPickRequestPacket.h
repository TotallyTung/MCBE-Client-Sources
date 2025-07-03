#pragma once

class ActorPickRequestPacket : public ::Packet {
public:
    int64_t         mID;       // this+0x30
    bool           mWithData; // this+0x38
    unsigned char  mMaxSlots; // this+0x39
};