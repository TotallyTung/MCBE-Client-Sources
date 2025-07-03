#pragma once
#include "Packet.h"

enum class InputData : uint64_t {
    Ascend = 1 << 0,
    Descend = 1 << 1,
    NorthJump = 1 << 2,
    JumpDown = 1 << 3,
    SprintDown = 1 << 4,
    ChangeHeight = 1 << 5,
    Jumping = 1 << 6,
    AutoJumpingInWater = 1 << 7,
    Sneaking = 1 << 8,
    SneakDown = 1 << 9,
    Up = 1 << 10,
    Down = 1 << 11,
    Left = 1 << 12,
    Right = 1 << 13,
    UpLeft = 1 << 14,
    UpRight = 1 << 15,
    WantUp = 1 << 16,
    WantDown = 1 << 17,
    WantDownSlow = 1 << 18,
    WantUpSlow = 1 << 19,
    Sprinting = 1 << 20,
    AscendBlock = 1 << 21,
    DescendBlock = 1 << 22,
    SneakToggleDown = 1 << 23,
    PersistSneak = 1 << 24,
    StartSprinting = 1 << 25,
    StopSprinting = 1 << 26,
    StartSneaking = 1 << 27,
    StopSneaking = 1 << 28,
    StartSwimming = 1 << 29,
    StopSwimming = 1 << 30,
    StartJumping = 1 << 31,
    StartGliding = 1ll << 32,
    StopGliding = 1ll << 33,
    PerformItemInteraction = 1ll << 34,
    PerformBlockActions = 1ll << 35,
    PerformItemStackRequest = 1ll << 36,
    HandledTeleport = 1ll << 37,
    Emoting = 1ll << 38,
    MissedSwing = 1ll << 39,
    StartCrawling = 1ll << 40,
    StopCrawling = 1ll << 41,
    StartFlying = 1ll << 42,
    StopFlying = 1ll << 43,
    ReceivedServerData = 1ll << 44,
    InClientPredictedInVehicle = 1ll << 45,
    PaddlingLeft = 1ll << 46,
    PaddlingRight = 1ll << 47,
};


inline InputData operator|(InputData a, InputData b) {
    return static_cast<InputData>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
}

inline InputData operator&(InputData a, InputData b) {
    return static_cast<InputData>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b));
}

inline InputData operator^(InputData a, InputData b) {
    return static_cast<InputData>(static_cast<uint64_t>(a) ^ static_cast<uint64_t>(b));
}

inline InputData operator~(InputData a) {
    return static_cast<InputData>(~static_cast<uint64_t>(a));
}

inline void operator|=(InputData& a, InputData b) {
    a = a | b;
}

inline void operator&=(InputData& a, InputData b) {
    a = a & b;
}

inline void operator^=(InputData& a, InputData b) {
    a = a ^ b;
}

inline bool operator!(InputData a) {
    return static_cast<uint64_t>(a) == 0;
}

enum class InputModeAuth : int {
    Undefined = 0x0,
    Mouse = 0x1,
    Touch = 0x2,
    GamePad = 0x3,
    MotionController = 0x4,
};

enum class ClientPlayMode : int {
    Normal = 0x0,
    Teaser = 0x1,
    Screen = 0x2,
    Viewer = 0x3,
    Reality = 0x4,
    Placement = 0x5,
    LivingRoom = 0x6,
    ExitLevel = 0x7,
    ExitLevelLivingRoom = 0x8,
    NumModes = 0x9,
};

class PlayerAuthInputPacket : public Packet {
public:
    /* Fields */
    /*Vector2<float> rotation; //0x0030
    Vector3<float> position; //0x0038
    float headYaw; //0x0044
    char pad_0048[4]; //0x0048
    float downVelocity; //0x004C
    char pad_0050[32]; //0x0050
    void* inputData; //0x0070
    void* inputMode; //0x0078
    void* playMode; //0x007C
    char pad_0080[8]; //0x0080
    int32_t ticksAlive; //0x0088
    char pad_008C[44]; //0x008C
    */
    Vec2<float>                                           rotation;             // this+0x30
    Vec3<float>                                           position;             // this+0x38
    float                                                    headYaw;              // this+0x44
    Vec3<float>                                           mPosDelta;            // this+0x4C
    Vec2<float>                                           mAnalogMoveVector;
    Vec2<float>                                           mVehicleRotation;
    Vec2<float>                                           mMove;
    Vec3<float>                                           mGazeDir;
    InputData                                                  mInputData;           // this+0x70
    InputModeAuth                                              mInputMode;           // this+0x78
    ClientPlayMode                                           mPlayMode;            // this+0x7C
    char                                                     pad_0080[8];          // this+0x80
    int32_t                                                  TicksAlive;           // this+0x88
    /*std::unique_ptr<PackedItemUseLegacyInventoryTransaction> mItemUseTransaction;
    std::unique_ptr<ItemStackRequestData>                    mItemStackRequest;
    PlayerBlockActions                                       mPlayerBlockActions;
    ActorUniqueID                                            mPredictedVehicle;*/

    /* Virtuals */
    /* Functions */
    void removeMovingInput()
    {
        mInputData &= ~(InputData::Up | InputData::Down | InputData::Left | InputData::Right | InputData::UpLeft | InputData::UpRight);
        mAnalogMoveVector = { 0, 0 };
        mMove = { 0, 0 };
    }
    PlayerAuthInputPacket() {};
    PlayerAuthInputPacket(const PlayerAuthInputPacket& other) : Packet()
    {
        memcpy(this, &other, sizeof(PlayerAuthInputPacket));
    }
    void operator=(const PlayerAuthInputPacket& other)
    {
        memcpy(this, &other, sizeof(PlayerAuthInputPacket));
    }
};