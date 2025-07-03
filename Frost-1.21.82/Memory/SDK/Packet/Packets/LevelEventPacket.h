#pragma once

class LevelEventPacket : public Packet {
public:
    LevelEvent     mEventId;  // this+0x30
    Vector3<float> mPos;      // this+0x34
    int            mData;     // this+0x40
};