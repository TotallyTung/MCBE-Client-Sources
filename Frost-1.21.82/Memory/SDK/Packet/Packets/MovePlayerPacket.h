#pragma once

class MovePlayerPacket : public Packet {
public:
    int                                        mPlayerID;         // this+0x30
    Vector3<float>                             mPos;              // this+0x38
    Vector2<float>                             mRot;              // this+0x44
    float                                      mYHeadRot;         // this+0x4C
    PlayerPositionModeComponent::PositionMode  mResetPosition;    // this+0x50
    bool                                       mOnGround;         // this+0x51
    int                                        mRidingID;         // this+0x58
    MinecraftEventing::TeleportationCause      mCause;            // this+0x60
    ActorType                                  mSourceEntityType; // this+0x64
    Tick                                       mTick;             // this+0x68
};