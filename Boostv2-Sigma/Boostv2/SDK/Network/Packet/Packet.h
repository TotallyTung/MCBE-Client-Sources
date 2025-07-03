#pragma once
#include "../../../Utils/MemoryUtil.h"
#include "../PacketHandlerDispatcherInstance.h"
#include "PacketIDs.h"
#include <string>
#include "../../../Utils/Maths.h"
class Packet {
public:
	CLASS_MEMBER(PacketHandlerDispatcherInstance*, packetHandler, 0x20);
private:
	char pad_0x0[0x30];
public:
	PacketID getId() {
		return MemoryUtil::CallVFunc<1, PacketID>(this);
	}

	std::string getName() {
		return *MemoryUtil::CallVFunc<2, std::string*>(this, std::string());
	}
};
enum class InteractAction
{
    RIGHT_CLICK = 1,
    LEFT_CLICK = 2,
    LEAVE_VEHICLE = 3,
    MOUSEOVER = 4
};

class InteractPacket : public ::Packet
{
public:
    static const PacketID ID = PacketID::Interact;

    // InteractPacket inner types define
    enum class Action : unsigned char {
        Invalid = 0x0,
        StopRiding = 0x3,
        InteractUpdate = 0x4,
        NpcOpen = 0x5,
        OpenInventory = 0x6,
    };

public:
    InteractAction mAction;   // this+0x30
    uint64_t               mTargetId; // this+0x38
    Vec3<float>                mPos;      // this+0x40

    // prevent constructor by default
    InteractPacket& operator=(InteractPacket const&);
    InteractPacket(InteractPacket const&);
};

enum class EffectType : unsigned int {
    Empty = 0,
    Speed = 1,
    Slowness = 2,
    Haste = 3,
    MiningFatigue = 4,
    Strength = 5,
    InstantHealth = 6,
    InstantDamage = 7,
    JumpBoost = 8,
    Nausea = 9,
    Regeneration = 10,
    Resistance = 11,
    FireResistance = 12,
    WaterBreathing = 13,
    Invisibility = 14,
    Blindness = 15,
    NightVision = 16,
    Hunger = 17,
    Weakness = 18,
    Poison = 19,
    Wither = 20,
    HealthBoost = 21,
    Absorption = 22,
    Saturation = 23,
    Levitation = 24,
    FatalPoison = 25,
    ConduitPower = 26,
    SlowFalling = 27,
    BadOmen = 28,
    VillageHero = 29,
    darkness = 30,
};

class MobEffectPacket : public ::Packet {
public:
    static const PacketID ID = PacketID::MobEffect;

    // MobEffectPacket inner types define
    enum class Event : signed char {
        Invalid = 0x0,
        Add = 0x1,
        Update = 0x2,
        Remove = 0x3,
    };

    uint64_t         mRuntimeId;           // this+0x30
    int              mEffectDurationTicks; // this+0x38
    enum class Event mEventId;             // this+0x3C
    EffectType       mEffectId;            // this+0x40
    int              mEffectAmplifier;     // this+0x44
    bool             mShowParticles;       // this+0x48
    uint64_t         mTick;                // this+0x50
};
