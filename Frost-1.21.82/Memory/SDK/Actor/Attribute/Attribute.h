#pragma once

#include "Amplifier.h"
#include "AttributeBuff.h"
#include "AttributeInstanceHandle.h"
#include "AttributeModifier.h"
#include "AttributeInstanceDelegate.h"
#include "ExhaustionAttributeDelegate.h"
#include "HealthAttributeDelegate.h"
#include "HungerAttributeDelegate.h"
#include "InstantaneousAttributeBuff.h"
#include "MultiplyValueAmplifier.h"
#include "ShiftedDurationAmplifier.h"
#include "ShiftedValueAmplifier.h"
#include "TemporalAttributeBuff.h"
#include "AttributeInstance.h"

class AttributeCollection;
struct AttributeData;

enum AttributeId {
    ZombieSpawnReinforcementsChange = -1,
    Health = 1,
    FollowRange = 2,
    KnockbackResistance = 3,
    MovementSpeed = 4,
    UnderwaterMovementSpeed = 5,
    LavaMovementSpeed = 6,
    AttackDamage = 7,
    Absorption = 8,
    Luck = 9,
    JumpStrength = 10, // for horse?
};

class Attribute {
public:
    __int64 hash;
    __int64 hashedStringHash;
    std::string attributeName;
private:
    char __padding[0x32];
public:

    Attribute() {
        memset(this, 0x0, sizeof(Attribute));
    }

    Attribute(__int64 hash) {
        memset(this, 0x0, sizeof(Attribute));
        this->hash = hash;
    }
};

enum AttributeHashes : unsigned __int64 {
    HEALTH = 30064771328 - 4294967296,
    HUNGER = 8589934848 - 4294967296,
    MOVEMENT = 42949673217 - 4294967296,
    ABSORPTION = 60129542401 - 4294967296,
    SATURATION = 12884902144 - 4294967296,
    FOLLOW_RANGE = 34359738369 - 4294967296,
    LEVEL = 21474836736 - 4294967296,
    EXPERIENCE = 25769804032 - 4294967296
};

class HealthAttribute : public Attribute
{
public:
    HealthAttribute() { this->hash = AttributeHashes::HEALTH; }
};

class PlayerHungerAttribute : public Attribute
{
public:
    PlayerHungerAttribute() { this->hash = AttributeHashes::HUNGER; }
};

class MovementAttribute : public Attribute
{
public:
    MovementAttribute() { this->hash = AttributeHashes::MOVEMENT; }
};

class AbsorptionAttribute : public Attribute
{
public:
    AbsorptionAttribute() { this->hash = AttributeHashes::ABSORPTION; }
};

class PlayerSaturationAttribute : public Attribute
{
public:
    PlayerSaturationAttribute() { this->hash = AttributeHashes::SATURATION; }
};

class FollowRangeAttribute : public Attribute
{
public:
    FollowRangeAttribute() { this->hash = AttributeHashes::FOLLOW_RANGE; }
};

class PlayerLevelAttribute : public Attribute {
public:
    PlayerLevelAttribute() { this->hash = AttributeHashes::LEVEL; }
};

class PlayerExperienceAttribute : public Attribute
{
public:
    PlayerExperienceAttribute() { this->hash = AttributeHashes::EXPERIENCE; }
};
