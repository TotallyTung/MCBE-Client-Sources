#pragma once

struct PackedItemUseLegacyInventoryTransaction {};
class  ItemStackRequestData {};

enum class AuthInputAction : uint64_t {
    NONE = 0,
    ASCEND = 1ULL << 0,
    DESCEND = 1ULL << 1,
    NORTH_JUMP_DEPRECATED = 1ULL << 2,
    JUMP_DOWN = 1ULL << 3,
    SPRINT_DOWN = 1ULL << 4,
    CHANGE_HEIGHT = 1ULL << 5,
    JUMPING = 1ULL << 6,
    AUTO_JUMPING_IN_WATER = 1ULL << 7,
    SNEAKING = 1ULL << 8,
    SNEAK_DOWN = 1ULL << 9,
    UP = 1ULL << 10,
    DOWN = 1ULL << 11,
    LEFT = 1ULL << 12,
    RIGHT = 1ULL << 13,
    UP_LEFT = 1ULL << 14,
    UP_RIGHT = 1ULL << 15,
    WANT_UP = 1ULL << 16,
    WANT_DOWN = 1ULL << 17,
    WANT_DOWN_SLOW = 1ULL << 18,
    WANT_UP_SLOW = 1ULL << 19,
    SPRINTING = 1ULL << 20,
    ASCEND_BLOCK = 1ULL << 21,
    DESCEND_BLOCK = 1ULL << 22,
    SNEAK_TOGGLE_DOWN = 1ULL << 23,
    PERSIST_SNEAK = 1ULL << 24,
    START_SPRINTING = 1ULL << 25,
    STOP_SPRINTING = 1ULL << 26,
    START_SNEAKING = 1ULL << 27,
    STOP_SNEAKING = 1ULL << 28,
    START_SWIMMING = 1ULL << 29,
    STOP_SWIMMING = 1ULL << 30,
    START_JUMPING = 1ULL << 31,
    START_GLIDING = 1ULL << 32,
    STOP_GLIDING = 1ULL << 33,
    PERFORM_ITEM_INTERACTION = 1ULL << 34,
    PERFORM_BLOCK_ACTIONS = 1ULL << 35,
    PERFORM_ITEM_STACK_REQUEST = 1ULL << 36,
    HANDLED_TELEPORT = 1ULL << 37,
    EMOTING = 1ULL << 38,
    MISSED_SWING = 1ULL << 39,
    START_CRAWLING = 1ULL << 40,
    STOP_CRAWLING = 1ULL << 41,
    START_FLYING = 1ULL << 42,
    STOP_FLYING = 1ULL << 43,
    RECEIVED_SERVER_DATA = 1ULL << 44,
    IN_CLIENT_PREDICTED_IN_VEHICLE = 1ULL << 45,
    PADDLING_LEFT = 1ULL << 46,
    PADDLING_RIGHT = 1ULL << 47,
    BLOCK_BREAKING_DELAY_ENABLED = 1ULL << 48,
    INPUT_NUM = 1ULL << 49,
};

inline AuthInputAction operator|(AuthInputAction a, AuthInputAction b) {
    return static_cast<AuthInputAction>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
}

inline AuthInputAction operator&(AuthInputAction a, AuthInputAction b) {
    return static_cast<AuthInputAction>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b));
}

inline AuthInputAction operator^(AuthInputAction a, AuthInputAction b) {
    return static_cast<AuthInputAction>(static_cast<uint64_t>(a) ^ static_cast<uint64_t>(b));
}

inline AuthInputAction operator~(AuthInputAction a) {
    return static_cast<AuthInputAction>(~static_cast<uint64_t>(a));
}

inline void operator|=(AuthInputAction& a, AuthInputAction b) {
    a = a | b;
}

inline void operator&=(AuthInputAction& a, AuthInputAction b) {
    a = a & b;
}

inline void operator^=(AuthInputAction& a, AuthInputAction b) {
    a = a ^ b;
}

inline bool operator!(AuthInputAction a) {
    return static_cast<uint64_t>(a) == 0;
}

class PlayerInputTick {
public:
    long mInputTick;
};

class PlayerAuthInputPacket : public Packet {
public:
    // PlayerAuthInputPacket inner types define
    enum class InputData : int {
        Ascend                     = 0x0,
        Descend                    = 0x1,
        NorthJump_Deprecated       = 0x2,
        JumpDown                   = 0x3,
        SprintDown                 = 0x4,
        ChangeHeight               = 0x5,
        Jumping                    = 0x6,
        AutoJumpingInWater         = 0x7,
        Sneaking                   = 0x8,
        SneakDown                  = 0x9,
        Up                         = 0xA,
        Down                       = 0xB,
        Left                       = 0xC,
        Right                      = 0xD,
        UpLeft                     = 0xE,
        UpRight                    = 0xF,
        WantUp                     = 0x10,
        WantDown                   = 0x11,
        WantDownSlow               = 0x12,
        WantUpSlow                 = 0x13,
        Sprinting                  = 0x14,
        AscendBlock                = 0x15,
        DescendBlock               = 0x16,
        SneakToggleDown            = 0x17,
        PersistSneak               = 0x18,
        StartSprinting             = 0x19,
        StopSprinting              = 0x1A,
        StartSneaking              = 0x1B,
        StopSneaking               = 0x1C,
        StartSwimming              = 0x1D,
        StopSwimming               = 0x1E,
        StartJumping               = 0x1F,
        StartGliding               = 0x20,
        StopGliding                = 0x21,
        PerformItemInteraction     = 0x22,
        PerformBlockActions        = 0x23,
        PerformItemStackRequest    = 0x24,
        HandledTeleport            = 0x25,
        Emoting                    = 0x26,
        MissedSwing                = 0x27,
        StartCrawling              = 0x28,
        StopCrawling               = 0x29,
        StartFlying                = 0x2A,
        StopFlying                 = 0x2B,
        ReceivedServerData         = 0x2C,
        InClientPredictedInVehicle = 0x2D,
        PaddlingLeft               = 0x2E,
        PaddlingRight              = 0x2F,
        BlockBreakingDelayEnabled  = 0x30,
        Input_Num                  = 0x31,
    };

public:
    Vector2<float>                                           mRotation;         
    Vector3<float>                                           mPosition;      
    float                                                    mYHeadYaw;        
    Vector3<float>                                           mPosDelta;          
    Vector2<float>                                           mVehicleRotation;
    Vector2<float>                                           mAnalogMoveVector;   
    Vector2<float>                                           mMove;              
    Vector2<float>                                           mInteractRots;       
    Vector3<float>                                           mCameraOrientation;
    Vector2<float>                                           mRawMoveVector;
    AuthInputAction                                          mInputData;
    InputMode                                                mInputMode;          
    ClientPlayMode                                           mPlayMode;           
    NewInteractionModel                                      mInteractionModel;  
    PlayerInputTick                                          mClientTick;         
    std::unique_ptr<PackedItemUseLegacyInventoryTransaction> mItemUseTransaction; 
    std::unique_ptr<ItemStackRequestData>                    mItemStackRequest;   
    PlayerBlockActions                                       mPlayerBlockActions; 
    uint64_t                                                 mPredictedVehicle;  

    void removeAllInputData() {
        mInputData = AuthInputAction::NONE;
    }
    void removeMovingInput()
    {
        mInputData &= ~(AuthInputAction::UP | AuthInputAction::DOWN | AuthInputAction::LEFT | AuthInputAction::RIGHT | AuthInputAction::UP_LEFT | AuthInputAction::UP_RIGHT);
        mAnalogMoveVector = { 0, 0 };
        mMove = { 0, 0 };
    }
    void addInputData(AuthInputAction data) {
        mInputData |= data;
    }
    bool hasInputData(AuthInputAction data) {
        return (mInputData & data) == data;
    }
    void addAllInputData() {
        for (int i = 0; i < static_cast<int>(InputData::Input_Num); i++) {
            // Continue if this input data isn't valid
            if (i == 2 || i == 27 || i == 31 || i == 0x22 || i == 0x23 || i == 0x24) {
                continue;
            }
            mInputData |= static_cast<AuthInputAction>(1ULL << i);
        }
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