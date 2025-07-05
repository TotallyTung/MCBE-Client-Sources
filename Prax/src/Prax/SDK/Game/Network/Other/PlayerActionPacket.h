#pragma once
//
// Created by vastrakai on 10/4/2023.
//

#include <magic_enum.hpp>
#include "src/Prax/SDK/Game/Network/Network.h"

/*
 * public static final int ACTION_START_BREAK = 0;
    public static final int ACTION_ABORT_BREAK = 1;
    public static final int ACTION_STOP_BREAK = 2;
    public static final int ACTION_GET_UPDATED_BLOCK = 3;
    public static final int ACTION_DROP_ITEM = 4;
    public static final int ACTION_START_SLEEPING = 5;
    public static final int ACTION_STOP_SLEEPING = 6;
    public static final int ACTION_RESPAWN = 7;
    public static final int ACTION_JUMP = 8;
    public static final int ACTION_START_SPRINT = 9;
    public static final int ACTION_STOP_SPRINT = 10;
    public static final int ACTION_START_SNEAK = 11;
    public static final int ACTION_STOP_SNEAK = 12;
    public static final int ACTION_CREATIVE_PLAYER_DESTROY_BLOCK = 13;
    public static final int ACTION_DIMENSION_CHANGE_ACK = 14; //sent when spawning in a different dimension to tell the server we spawned
    public static final int ACTION_START_GLIDE = 15;
    public static final int ACTION_STOP_GLIDE = 16;
    public static final int ACTION_BUILD_DENIED = 17;
    public static final int ACTION_CONTINUE_BREAK = 18;
    public static final int ACTION_SET_ENCHANTMENT_SEED = 20;
    public static final int ACTION_START_SWIMMING = 21;
    public static final int ACTION_STOP_SWIMMING = 22;
    public static final int ACTION_START_SPIN_ATTACK = 23;
    public static final int ACTION_STOP_SPIN_ATTACK = 24;
    public static final int ACTION_INTERACT_BLOCK = 25;
    public static final int ACTION_PREDICT_DESTROY_BLOCK = 26;
    public static final int ACTION_CONTINUE_DESTROY_BLOCK = 27;
    public static final int ACTION_START_ITEM_USE_ON = 28;
    public static final int ACTION_STOP_ITEM_USE_ON = 29;

    public long entityId;
    public int action;
    public int x;
    public int y;
    public int z;
    public BlockVector3 resultPosition;
    public int face;
 */

enum class PlayerAction : int {
    Unknown                          = -1,
    StartDestroyBlock                = 0x0,
    AbortDestroyBlock                = 0x1,
    StopDestroyBlock                 = 0x2,
    GetUpdatedBlock                  = 0x3,
    DropItem                         = 0x4,
    StartSleeping                    = 0x5,
    StopSleeping                     = 0x6,
    Respawn                          = 0x7,
    StartJump                        = 0x8,
    StartSprinting                   = 0x9,
    StopSprinting                    = 0xA,
    StartSneaking                    = 0xB,
    StopSneaking                     = 0xC,
    CreativeDestroyBlock             = 0xD,
    ChangeDimensionAck               = 0xE,
    StartGliding                     = 0xF,
    StopGliding                      = 0x10,
    DenyDestroyBlock                 = 0x11,
    CrackBlock                       = 0x12,
    ChangeSkin                       = 0x13,
    DEPRECATED_UpdatedEnchantingSeed = 0x14,
    StartSwimming                    = 0x15,
    StopSwimming                     = 0x16,
    StartSpinAttack                  = 0x17,
    StopSpinAttack                   = 0x18,
    InteractWithBlock                = 0x19,
    PredictDestroyBlock              = 0x1A,
    ContinueDestroyBlock             = 0x1B,
    StartItemUseOn                   = 0x1C,
    StopItemUseOn                    = 0x1D,
    HandledTeleport                  = 0x1E,
    Count                            = 0x1F,
};

class PlayerActionPacket : public Packet {
public:
    static const PacketID ID = PacketID::PlayerAction;

    BlockPos             mPos;                               // this+0x30
    BlockPos             mResultPos;                         // this+0x3C
    int                  mFace;                              // this+0x48
    PlayerAction         mAction;                            // this+0x4C
    uint64_t             mRuntimeId;                         // this+0x50
    bool                 mtIsFromServerPlayerMovementSystem; // this+0x58

    static std::shared_ptr<PlayerActionPacket> Create(PlayerAction action, BlockPos pos, BlockPos resultPos, int face, uint64_t runtimeId, bool isFromServerPlayerMovementSystem) {
        auto packet = MinecraftPackets::CreatePacket<PlayerActionPacket>();
        packet->mAction = action;
        packet->mPos = pos;
        packet->mResultPos = resultPos;
        packet->mFace = face;
        packet->mRuntimeId = runtimeId;
        packet->mtIsFromServerPlayerMovementSystem = isFromServerPlayerMovementSystem;
        return packet;
    }

    std::string GetActionType() {
        // Use magic enum to convert the enum to a string
        return std::string(magic_enum::enum_name(mAction));
    }
};
