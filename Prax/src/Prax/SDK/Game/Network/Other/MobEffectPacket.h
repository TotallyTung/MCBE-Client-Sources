//
// Created by vastrakai on 5/5/2024.
//

#pragma once
#include <magic_enum.hpp>
#include <Prax/SDK/Game/Network/Network.h>


enum class EffectType : unsigned int {
    Empty          = 0,
    Speed          = 1,
    Slowness       = 2,
    Haste          = 3,
    MiningFatigue  = 4,
    Strength       = 5,
    InstantHealth  = 6,
    InstantDamage  = 7,
    JumpBoost      = 8,
    Nausea         = 9,
    Regeneration   = 10,
    Resistance     = 11,
    FireResistance = 12,
    WaterBreathing = 13,
    Invisibility   = 14,
    Blindness      = 15,
    NightVision    = 16,
    Hunger         = 17,
    Weakness       = 18,
    Poison         = 19,
    Wither         = 20,
    HealthBoost    = 21,
    Absorption     = 22,
    Saturation     = 23,
    Levitation     = 24,
    FatalPoison    = 25,
    ConduitPower   = 26,
    SlowFalling    = 27,
    BadOmen        = 28,
    VillageHero    = 29,
    darkness       = 30,
};

class MobEffectPacket : public ::Packet {
public:
    static const PacketID ID = PacketID::MobEffect;

    // MobEffectPacket inner types define
    enum class Event : signed char {
        Invalid = 0x0,
        Add     = 0x1,
        Update  = 0x2,
        Remove  = 0x3,
    };

    uint64_t         mRuntimeId;           // this+0x30
    int              mEffectDurationTicks; // this+0x38
    enum class Event mEventId;             // this+0x3C
    EffectType       mEffectId;            // this+0x40
    int              mEffectAmplifier;     // this+0x44
    bool             mShowParticles;       // this+0x48
    uint64_t         mTick;                // this+0x50

    std::string_view GetEffectName()
    {
        // use magic_enum
        return magic_enum::enum_name<EffectType>(mEffectId);
    }

    std::string_view GetEventName()
    {
        // use magic_enum
        return magic_enum::enum_name<Event>(mEventId);
    }
};
