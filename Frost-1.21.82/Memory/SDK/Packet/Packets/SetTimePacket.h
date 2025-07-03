#pragma once

class SetTimePacket : public ::Packet {
public:
    int mTime; // this+0x30
};