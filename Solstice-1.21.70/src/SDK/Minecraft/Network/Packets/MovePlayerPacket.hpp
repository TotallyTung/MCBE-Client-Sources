//
// Created by vastrakai on 7/3/2024.
//

#pragma once

#include "Packet.hpp"
#include <SDK/Minecraft/Actor/ActorType.hpp>


enum class PositionMode : unsigned char {
    Normal      = 0,
    Respawn     = 1,
    Teleport    = 2,
    OnlyHeadRot = 3,
};

enum class TeleportationCause : int {
    Unknown     = 0x0,
    Projectile  = 0x1,
    ChorusFruit = 0x2,
    Command     = 0x3,
    Behavior    = 0x4,
    Count       = 0x5,
};

class MovePlayerPacket : public Packet {
public:
    static inline const auto ID = PacketID::MovePlayer;

    int64_t                                   mPlayerID;         // this+0x30
    glm::vec3                                 mPos;              // this+0x38
    glm::vec2                                 mRot;              // this+0x44
    float                                     mYHeadRot;         // this+0x4C
    PositionMode                              mResetPosition;    // this+0x50
    bool                                      mOnGround;         // this+0x51
    int64_t                                   mRidingID;         // this+0x58
    TeleportationCause                        mCause;            // this+0x60
    ActorType                                 mSourceEntityType; // this+0x64
    uint64_t                                  mTick;             // this+0x68
};


class MoveActorAbsoluteData
{
public:
    union Header {
        unsigned char mRaw;                  // this+0x0
        bool  mIsOnGround;           // this(bf)+0x0:0x0 len(0x1)
        bool  mTeleported;           // this(bf)+0x0:0x1 len(0x1)
        bool  mForceMoveLocalEntity; // this(bf)+0x0:0x2 len(0x1)
    };

    uint64_t                      mRuntimeId;
    MoveActorAbsoluteData::Header mHeader;
    glm::vec3                     mPos;
    signed char                   mRotX;
    signed char                   mRotY;
    signed char                   mRotYHead;
    signed char                   mRotYBody;
};

class MoveActorDeltaData
{
public:
    union Header {
        unsigned short mRaw;                   // this+0x0
        bool   mContainsPositionX;     // this(bf)+0x0:0x0 len(0x1)
        bool   mContainsPositionY;     // this(bf)+0x0:0x1 len(0x1)
        bool   mContainsPositionZ;     // this(bf)+0x0:0x2 len(0x1)
        bool   mContainsRotationX;     // this(bf)+0x0:0x3 len(0x1)
        bool   mContainsRotationY;     // this(bf)+0x0:0x4 len(0x1)
        bool   mContainsRotationYHead; // this(bf)+0x0:0x5 len(0x1)
        bool   mIsOnGround;            // this(bf)+0x0:0x6 len(0x1)
        bool   mTeleported;            // this(bf)+0x0:0x7 len(0x1)
        bool   mForceMoveLocalEntity;  // this(bf)+0x1:0x0 len(0x1)
    };

    uint64_t                   mRuntimeId;
    MoveActorDeltaData::Header mHeader;
    float                      mNewPositionX;
    float                      mNewPositionY;
    float                      mNewPositionZ;
    signed char                mRotX;
    signed char                mRotY;
    signed char                mRotYHead;
    MoveActorAbsoluteData      mPreviousData;
};

class MoveActorDeltaPacket : public ::Packet
{
public:
    MoveActorDeltaData mMoveData; // this+0x30
};

class MoveActorAbsolutePacket : public ::Packet
{
public:
    MoveActorAbsoluteData mMoveData; // this+0x30
};