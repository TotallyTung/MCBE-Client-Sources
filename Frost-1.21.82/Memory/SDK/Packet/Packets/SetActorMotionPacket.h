#pragma once

class SetActorMotionPacket : public Packet {
public:
    int             mRuntimeId; // this+0x30
    Vector3<float>  mMotion;    // this+0x38
    int             mTick;      // this+0x48
};