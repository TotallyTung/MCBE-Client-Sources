#pragma once

#include "src/Prax/SDK/Game/Network/Network.h"

enum class AuthInputMode : int {
    UNDEFINED = 0,
    MOUSE = 1,
    TOUCH = 2,
    GAME_PAD = 3,
    MOTION_CONTROLLER = 4,
    COUNT = 5,
    INVALID = 6
};

enum class AuthInputAction : uint64_t {
    NONE = 0,
    ASCEND = 1ULL << 0,
    DESCEND = 1ULL << 1,
    NORTH_JUMP = 1ULL << 2,
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
    ASCEND_SCAFFOLDING = 1ULL << 21,
    DESCEND_SCAFFOLDING = 1ULL << 22,
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
    HANDLE_TELEPORT = 1ULL << 37,
    EMOTING = 1ULL << 38,
    MISSED_SWING = 1ULL << 39,
    START_CRAWLING = 1ULL << 40,
    STOP_CRAWLING = 1ULL << 41,
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

enum class ClientPlayMode {
    NORMAL,
    TEASER,
    SCREEN,
    VIEWER,
    REALITY,
    PLACEMENT,
    LIVING_ROOM,
    EXIT_LEVEL,
    EXIT_LEVEL_LIVING_ROOM,
    INVALID
};

/*public enum AuthInteractionModel {
    TOUCH,
    CROSSHAIR,
    CLASSIC;
    */
enum class AuthInteractionModel {
    TOUCH,
    CROSSHAIR,
    CLASSIC
};
/* fields from PowerNukkitX
    private float yaw;
    private float pitch;
    private float headYaw;
    private Vector3f position;
    private Vector2 motion;
    private Set<AuthInputAction> inputData = EnumSet.noneOf(AuthInputAction.class);
    private InputMode inputMode;
    private ClientPlayMode playMode;
    private AuthInteractionModel interactionModel;
    private Vector3f vrGazeDirection;
    private long tick;
    private Vector3f delta;
    // private ItemStackRequest itemStackRequest;
    private Map<PlayerActionType, PlayerBlockActionData> blockActionData = new EnumMap<>(PlayerActionType.class);
    @Since("1.19.70-r1")
    private Vector2f analogMoveVector;
*/

/*

public enum PlayerActionType {

    START_DESTROY_BLOCK,
    ABORT_DESTROY_BLOCK,
    STOP_DESTROY_BLOCK,
    GET_UPDATED_BLOCK,
    DROP_ITEM,
    START_SLEEPING,
    STOP_SLEEPING,
    RESPAWN,
    START_JUMP,
    START_SPRINTING,
    STOP_SPRINTING,
    START_SNEAKING,
    STOP_SNEAKING,
    CREATIVE_DESTROY_BLOCK,
    CHANGE_DIMENSION_ACK,
    START_GLIDING,
    STOP_GLIDING,
    DENY_DESTROY_BLOCK,
    CRACK_BLOCK,
    CHANGE_SKIN,
    UPDATED_ENCHANTING_SEED,
    START_SWIMMING,
    STOP_SWIMMING,
    START_SPIN_ATTACK,
    STOP_SPIN_ATTACK,
    INTERACT_WITH_BLOCK,
    PREDICT_DESTROY_BLOCK,
    CONTINUE_DESTROY_BLOCK,
    START_ITEM_USE_ON,
    STOP_ITEM_USE_ON,
    HANDLED_TELEPORT,
    MISSED_SWING,
    START_CRAWLING,
    STOP_CRAWLING,
    START_FLYING,
    STOP_FLYING;
}
 */

enum class PlayerActionType {
    START_DESTROY_BLOCK,
    ABORT_DESTROY_BLOCK,
    STOP_DESTROY_BLOCK,
    GET_UPDATED_BLOCK,
    DROP_ITEM,
    START_SLEEPING,
    STOP_SLEEPING,
    RESPAWN,
    START_JUMP,
    START_SPRINTING,
    STOP_SPRINTING,
    START_SNEAKING,
    STOP_SNEAKING,
    CREATIVE_DESTROY_BLOCK,
    CHANGE_DIMENSION_ACK,
    START_GLIDING,
    STOP_GLIDING,
    DENY_DESTROY_BLOCK,
    CRACK_BLOCK,
    CHANGE_SKIN,
    UPDATED_ENCHANTING_SEED,
    START_SWIMMING,
    STOP_SWIMMING,
    START_SPIN_ATTACK,
    STOP_SPIN_ATTACK,
    INTERACT_WITH_BLOCK,
    PREDICT_DESTROY_BLOCK,
    CONTINUE_DESTROY_BLOCK,
    START_ITEM_USE_ON,
    STOP_ITEM_USE_ON,
    HANDLED_TELEPORT,
    MISSED_SWING,
    START_CRAWLING,
    STOP_CRAWLING,
    START_FLYING,
    STOP_FLYING
};

/*
public class PlayerBlockActionData {
    private PlayerActionType action;
    private BlockVector3 position;
    private int facing;
}
 */

class PlayerBlockActionData {
public:
    PlayerActionType action;
    BlockPos position;
    int facing;
};

struct PackedItemUseLegacyInventoryTransaction {
    PackedItemUseLegacyInventoryTransaction& operator=(PackedItemUseLegacyInventoryTransaction const&);
    PackedItemUseLegacyInventoryTransaction(PackedItemUseLegacyInventoryTransaction const&);
};

class ItemStackRequestData {
    ItemStackRequestData& operator=(ItemStackRequestData const&);
    ItemStackRequestData(ItemStackRequestData const&);
    ItemStackRequestData();
};

class PlayerBlockActions {
public:
    std::vector<PlayerBlockActionData> mActions;
};

class PlayerAuthInputPacket : Packet {
public:
    static const PacketID ID = PacketID::PlayerAuthInput;

    Vector2 rotation; //0x0030
    Vector3 position; //0x0038
    float headYaw; //0x0044
    Vector3 velocity; //0x0048
    Vector2 vehicleRot; //0x0054
    Vector2 analogMoveVector;
    Vector2 moveVector; //0x005C
    char pad_0050[12]; //0x0054
    AuthInputAction inputFlags; //0x0070
    AuthInputMode inputMode; //0x0078
    ClientPlayMode clientPlayMode;
    AuthInteractionModel interactionModel; // 0x007C (size: 4)
    uint64_t ticksAlive; //0x0088
    std::unique_ptr<PackedItemUseLegacyInventoryTransaction> itemUseTransaction;
    std::unique_ptr<ItemStackRequestData> itemStackRequest;
    PlayerBlockActions playerBlockActions;

    std::string toString() {
        // for now just show the rotation, position, and headYaw
        std::string str = "PlayerAuthInputPacket: ";
        str += "Rotation: " + std::to_string(rotation.x) + ", " + std::to_string(rotation.y) + "\n";
        str += "Position: " + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + "\n";
        str += "HeadYaw: " + std::to_string(headYaw) + "\n";
        str += "Velocity: " + std::to_string(velocity.x) + ", " + std::to_string(velocity.y) + ", " + std::to_string(velocity.z) + "\n";
        str += "analogueMoveVector: " + std::to_string(analogMoveVector.x) + ", " + std::to_string(analogMoveVector.y) + "\n";
        str += "moveVector: " + std::to_string(moveVector.x) + ", " + std::to_string(moveVector.y) + "\n";
        str += "inputMode: " + std::to_string((int)inputMode) + "\n";
        str += "clientPlayMode: " + std::to_string((int)clientPlayMode) + "\n";
        str += "interactionModel: " + std::to_string((int)interactionModel) + "\n";
        str += "ticksAlive: " + std::to_string(ticksAlive) + "\n";

        bool hasBlockActions = (inputFlags & AuthInputAction::PERFORM_BLOCK_ACTIONS) == AuthInputAction::PERFORM_BLOCK_ACTIONS;
        if (hasBlockActions){
            str += "(CONTAINS BLOCK ACTIONS)\n";
        }
        return str;
    }

    // please use CreatePacket instead
    PlayerAuthInputPacket() = default;

    PlayerAuthInputPacket(const PlayerAuthInputPacket& other) : Packet()
    {
        memcpy(this, &other, sizeof(PlayerAuthInputPacket));
    }
    void operator=(const PlayerAuthInputPacket& other)
    {
        memcpy(this, &other, sizeof(PlayerAuthInputPacket));
    }

    void CopyFrom(PlayerAuthInputPacket* other) {
        rotation = other->rotation;
        position = other->position;
        headYaw = other->headYaw;
        velocity = other->velocity;
        vehicleRot = other->vehicleRot;
        analogMoveVector = other->analogMoveVector;
        moveVector = other->moveVector;
        inputFlags = other->inputFlags;
        inputMode = other->inputMode;
        clientPlayMode = other->clientPlayMode;
        interactionModel = other->interactionModel;
        ticksAlive = other->ticksAlive;
        playerBlockActions = other->playerBlockActions;
        playerBlockActions.mActions = other->playerBlockActions.mActions;

    }
};




