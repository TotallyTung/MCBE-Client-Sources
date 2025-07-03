#pragma once

class MobEffectPacket : public Packet {
public:
    // MobEffectPacket inner types define
    enum class Event : signed char {
        Invalid = 0x0,
        Add     = 0x1,
        Update  = 0x2,
        Remove  = 0x3,
    };

    uint64_t              mRuntimeId;           // this+0x30
    int                   mEffectDurationTicks; // this+0x38
    enum class Event      mEventId;             // this+0x3C
    MobEffect::EffectType mEffectId;            // this+0x40
    int                   mEffectAmplifier;     // this+0x44
    bool                  mShowParticles;       // this+0x48
    uint64_t              mTick;                // this+0x50
};