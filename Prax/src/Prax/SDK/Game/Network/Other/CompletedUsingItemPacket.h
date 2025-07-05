//
// Created by vastrakai on 10/17/2023.
//

#pragma once


#include "Prax/SDK/Game/Network/Network.h"

class CompletedUsingItemPacket : public Packet {
public:
    int ItemId;
    int Action;
};

class SetHealthPacket : public Packet {
public:
    int Health;
};

/* from powernukkitx
    public static final int STATE_SEARCHING_FOR_SPAWN = 0;
    public static final int STATE_READY_TO_SPAWN = 1;
    public static final int STATE_CLIENT_READY_TO_SPAWN = 2;
*/
enum class RespawnState {
    STATE_SEARCHING_FOR_SPAWN = 0,
    STATE_READY_TO_SPAWN = 1,
    STATE_CLIENT_READY_TO_SPAWN = 2
};
class RespawnPacket : public Packet {
public:
    Vector3 Position;
    int RespawnState;
    uint64_t EntityRuntimeId;

};

enum class PlayStatus : int {
    LoginSuccess                              = 0x0,
    LoginFailed_ClientOld                     = 0x1,
    LoginFailed_ServerOld                     = 0x2,
    PlayerSpawn                               = 0x3,
    LoginFailed_InvalidTenant                 = 0x4,
    LoginFailed_EditionMismatchEduToVanilla   = 0x5,
    LoginFailed_EditionMismatchVanillaToEdu   = 0x6,
    LoginFailed_ServerFullSubClient           = 0x7,
    LoginFailed_EditorMismatchEditorToVanilla = 0x8,
    LoginFailed_EditorMismatchVanillaToEditor = 0x9,
};

class PlayStatusPacket : public Packet {
public:
    PlayStatus Status;
};


class SetLocalPlayerAsInitializedPacket : public ::Packet {
public:
    uint64_t EntityRuntimeId;

};

class MoveActorAbsolutePacket : public Packet {
public:
    static const PacketID ID = PacketID::MoveEntityAbsolute;
    /* java code
     public long eid;
    public double x;
    public double y;
    public double z;
    public double yaw;
    public double headYaw;
    public double pitch;
    public boolean onGround;
    public boolean teleport;
    public boolean forceMoveLocalEntity;*/
    union Header {
        unsigned char mRaw;                  // this+0x0
        bool  mIsOnGround;           // this(bf)+0x0:0x0 len(0x1)
        bool  mTeleported;           // this(bf)+0x0:0x1 len(0x1)
        bool  mForceMoveLocalEntity; // this(bf)+0x0:0x2 len(0x1)
    };

    uint64_t                mRuntimeId;
    Header mHeader;
    Vector3                          mPos;
    signed char                         mRotX;
    signed char                         mRotY;
    signed char                         mRotYHead;
    signed char                         mRotYBody;

    float GetYaw() const {
        return mRotY;
    }

    float GetHeadYaw() const {
        return mRotYHead;
    }

};