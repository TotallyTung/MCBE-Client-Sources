//
// Created by vastrakai on 10/15/2023.
//

#pragma once

#include <cstdint>
#include "Prax/SDK/Game/Network/Network.h"

enum class ActorLinkType : unsigned char {
    None      = 0x0,
    Riding    = 0x1,
    Passenger = 0x2,
};

struct ActorUniqueID {
    int64_t mID; // this+0x0
};

struct ActorLink
{
    ActorLinkType type;                // this+0x0
    ActorUniqueID A;                   // this+0x8
    ActorUniqueID B;                   // this+0x10
    bool          mImmediate;          // this+0x18
    bool          mPassengerInitiated; // this+0x19
};

struct ActorDefinitionIdentifier
{
public:
    std::string  mNamespace;     // 0
    std::string  mIdentifier;    // 32
    std::string  mInitEvent;     // 64
    std::string  mFullName;      // 96
    HashedString mCanonicalName; // 128
};

struct PropertySyncData
{
public:
    struct PropertySyncIntEntry {
        unsigned int mPropertyIndex;
        int  mData;
    };

    struct PropertySyncFloatEntry {
        unsigned int mPropertyIndex;
        float mData;
    };

    std::vector<PropertySyncData::PropertySyncIntEntry>   mIntEntries;
    std::vector<PropertySyncData::PropertySyncFloatEntry> mFloatEntries;
};
class AttributeInstanceHandle
{
public:
    // prevent constructor by default
    AttributeInstanceHandle& operator=(AttributeInstanceHandle const&);
    AttributeInstanceHandle(AttributeInstanceHandle const&);
    AttributeInstanceHandle();
};


struct SyncedAttribute
{
public:
    std::string mName;         // this+0x0
    float       mMinValue;     // this+0x20
    float       mCurrentValue; // this+0x24
    float       mMaxValue;     // this+0x28
};


class AddActorPacket : public Packet
{
public:
    static const PacketID ID = PacketID::AddEntity;
    std::vector<ActorLink>                 mLinks;
    Vector3                                mPos;
    Vector3                                mVelocity;
    Vector2                                mRot;
    float                                  mYHeadRotation;
    float                                  mYBodyRotation;
    ActorUniqueID                          mEntityId;
    uint64_t                               mRuntimeId;
    void*                                  mEntityData;
    std::vector<std::unique_ptr<void>>     mData;
    ActorDefinitionIdentifier              mType;
    PropertySyncData                       mSynchedProperties;
    std::vector<AttributeInstanceHandle>   mAttributeHandles;
    std::vector<SyncedAttribute>           mAttributes;
};

class RemoveActorPacket : public Packet {
public:
    static const PacketID ID = PacketID::RemoveEntity;
    int64_t ActorRuntimeID;
};