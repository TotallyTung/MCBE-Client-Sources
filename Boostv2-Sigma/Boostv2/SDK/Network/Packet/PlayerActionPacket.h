#pragma once

#include "Packet.h"
#include "NetworkBlockPosition.h"
class PlayerActionPacket : public Packet {
public:
    enum class PlayerActionType : int {
        Unknown = -1,
        StartDestroyBlock = 0x0,
        AbortDestroyBlock = 0x1,
        StopDestroyBlock = 0x2,
        GetUpdatedBlock = 0x3,
        DropItem = 0x4,
        StartSleeping = 0x5,
        StopSleeping = 0x6,
        Respawn = 0x7,
        StartJump = 0x8,
        StartSprinting = 0x9,
        StopSprinting = 0xA,
        StartSneaking = 0xB,
        StopSneaking = 0xC,
        CreativeDestroyBlock = 0xD,
        ChangeDimensionAck = 0xE,
        StartGliding = 0xF,
        StopGliding = 0x10,
        DenyDestroyBlock = 0x11,
        CrackBlock = 0x12,
        ChangeSkin = 0x13,
        DEPRECATED_UpdatedEnchantingSeed = 0x14,
        StartSwimming = 0x15,
        StopSwimming = 0x16,
        StartSpinAttack = 0x17,
        StopSpinAttack = 0x18,
        InteractWithBlock = 0x19,
        PredictDestroyBlock = 0x1A,
        ContinueDestroyBlock = 0x1B,
        StartItemUseOn = 0x1C,
        StopItemUseOn = 0x1D,
        HandledTeleport = 0x1E,
        Count = 0x1F,
    };

	NetworkBlockPosition mPos;                               // this+0x30
	NetworkBlockPosition mResultPos;                         // this+0x3C
	int                  mFace;                              // this+0x48
	PlayerActionType     mAction;                            // this+0x4C
	int                  mRuntimeId;                         // this+0x50
	bool                 mtIsFromServerPlayerMovementSystem; // this+0x58
};