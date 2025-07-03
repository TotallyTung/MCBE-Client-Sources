//
// Created by vastrakai on 9/23/2024.
//

#include "LocalPlayerLib.hpp"

#include <magic_enum.hpp>
#include <Hook/Hooks/NetworkHooks/PacketSendHook.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <LuaBridge/LuaBridge.h>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/Actor/SyncedPlayerMovementSettings.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include "lua.h"

class ActorVectorWrapper
{
public:
    ActorVectorWrapper(std::vector<Actor*>* vec)
    {
        vector = *std::move(vec);
    }

    // Function to get the size (for # operator)
    [[nodiscard]] size_t size() const
    {
        return vector.size();
    }

    // Function to get an actor at a specific index (1-based for Lua)
    Actor* get(size_t index)
    {
        if (index < 1 || index > vector.size())
        {
            return nullptr;
        }
        return (vector)[index - 1];
    }

    // Overload the operator[] to support Lua-style indexing (1-based)
    Actor* operator[](size_t index)
    {
        return get(index);
    }

private:
    std::vector<Actor*> vector;
};

void LocalPlayerLib::initialize(lua_State* L)

/*
 *
 *void swing();
    bool isDestroying();
    bool isSwinging();
    void setSwinging(bool swinging);
    int getGameType();
    void setGameType(int type);
    bool isDebugCameraActive();
    void setDebugCameraActive(bool active);
    void setAllowInsideBlockRender(bool allow);
    int getSwingProgress();
    int getOldSwingProgress();
    void setSwingProgress(int progress);
    AABB getAABB(bool normal = false);
    void setAABB(AABB& aabb);
    bool isPlayer();
    ChunkPos getChunkPos();
    glm::vec3* getPos();
    glm::vec3* getPosPrev();
    class GameMode* getGameMode();
    ActorWalkAnimationComponent* getWalkAnimationComponent();
    DebugCameraComponent* getDebugCameraComponent();
    CameraPresetComponent* getCameraPresetComponent();
    ActorTypeComponent* getActorTypeComponent();
    RenderPositionComponent* getRenderPositionComponent();
    int64_t getRuntimeID();
    StateVectorComponent* getStateVectorComponent();
    MoveInputComponent* getMoveInputComponent();
    ActorRotationComponent* getActorRotationComponent();
    AABBShapeComponent* getAABBShapeComponent();
    BlockMovementSlowdownMultiplierComponent* getBlockMovementSlowdownMultiplierComponent();
    ActorUniqueIDComponent* getActorUniqueIDComponent();
    ContainerManagerModel* getContainerManagerModel();
    ActorHeadRotationComponent* getActorHeadRotationComponent();
    MobBodyRotationComponent* getMobBodyRotationComponent();
    JumpControlComponent* getJumpControlComponent();
    CameraComponent* getCameraComponent();
    CameraDirectLookComponent* getCameraDirectLookComponent();
    MaxAutoStepComponent* getMaxAutoStepComponent();
    MobHurtTimeComponent* getMobHurtTimeComponent();
    class SimpleContainer* getArmorContainer();
    class PlayerInventory* getSupplies();
    class Level* getLevel();
    class SerializedSkin* getSkin();
    void setPosition(glm::vec3 pos);
    void setPos(float x, float y, float z);
    float distanceTo(Actor* actor);
    float distanceTo(const glm::vec3& pos);
    bool wasOnGround();
    bool isOnGround();
    void setOnGround(bool);
    bool isCollidingHorizontal();
    void jumpFromGround();
    float getFallDistance();
    void setFallDistance(float distance);
    std::string getRawName();
    const std::string& getNameTag();
    void setNametag(const std::string& name);
    AttributesComponent* getAttributesComponent();
    float getMaxHealth();
    float getHealth();
    float getAbsorption();
    float getMaxAbsorption();
    AttributeInstance* getAttribute(AttributeId id);
    AttributeInstance* getAttribute(int id);
    bool isOnFire();
    std::string getLocalName();
    std::string getXuid();
    bool isValid();
class GameMode {
public:
    CLASS_FIELD(class Actor*, mPlayer, 0x8);
    CLASS_FIELD(float, mBreakProgress, 0x24);

    // realistically, i don't think this vTable will ever change so we can leave it here lol
    virtual ~GameMode() = 0;
    virtual void startDestroyBlock(glm::ivec3* position, int blockSide, bool& isDestroyedOut) = 0;
    virtual void destroyBlock(glm::ivec3* position, int blockSide) = 0;
    virtual void continueDestroyBlock(glm::ivec3 const& position, unsigned int blockSide, glm::vec3 const& playerPosition, bool&) = 0;
    virtual void stopDestroyBlock(glm::ivec3 const& position) = 0;
    virtual void startBuildBlock(glm::ivec3 const& position, unsigned char blockSide) = 0;
    virtual void buildBlock(glm::ivec3 const&, unsigned char blockSide, bool) = 0;
    virtual void continueBuildBlock(glm::ivec3 const& position, unsigned char blockSide) = 0;
    virtual void stopBuildBlock() = 0;
    virtual void tick() = 0;
    virtual float getPickRange(int inputMode, bool) = 0;
    virtual void useItem(class ItemStack* targetStack) = 0;
    virtual void useItemOn(ItemStack* targetStack, glm::ivec3 const& blockPosition, int blockSide, glm::vec3 const& playerPosition, class Block const* blockClass) = 0;
    virtual void interact(class Actor* target, glm::vec3 const& position) = 0;
    virtual void attack(Actor* target) = 0;
    virtual void releaseUsingItem() = 0;

    float getDestroyRate(const class Block& block);
    bool baseUseItem(class ItemStack* itemStack);
};
*/
{
    getGlobalNamespace(L)
        .beginClass<Actor>("Actor")
        .addFunction("swing", &Actor::swing)
        .addFunction("isDestroying", &Actor::isDestroying)
        .addFunction("isSwinging", &Actor::isSwinging)
        .addFunction("setGameType", &Actor::setGameType)
        .addFunction("getGameType", &Actor::getGameType)
        .addFunction("getActorType", [](Actor* actor) -> std::string
        {
            auto typeComponent = actor->getActorTypeComponent();
            if (typeComponent == nullptr)
            {
                return "Unknown";
            }
            return std::string(magic_enum::enum_name(typeComponent->mType));
        })
        .addFunction("getSwingProgress", &Actor::getSwingProgress)
        .addFunction("getOldSwingProgress", &Actor::getOldSwingProgress)
        .addFunction("setSwingProgress", &Actor::setSwingProgress)
        .addFunction("getAABB", &Actor::getAABB)
        .addFunction("setAABB", &Actor::setAABB)
        .addFunction("isPlayer", &Actor::isPlayer)
        .addFunction("getChunkPos", &Actor::getChunkPos)
        .addFunction("getRuntimeID", &Actor::getRuntimeID)
        .addFunction("wasOnGround", &Actor::wasOnGround)
        .addFunction("isOnGround", &Actor::isOnGround)
        .addFunction("setOnGround", &Actor::setOnGround)
        .addFunction("isCollidingHorizontal", &Actor::isCollidingHorizontal)
        .addFunction("jumpFromGround", [](Actor* actor)
        {
            actor->jumpFromGround();
            PacketSendHook::mApplyJump = true;
        })
        .addFunction("getFallDistance", &Actor::getFallDistance)
        .addFunction("setFallDistance", &Actor::setFallDistance)
        .addFunction("getRawName", &Actor::getRawName)
        .addFunction("getNameTag", &Actor::getNameTag)
        .addFunction("setNametag", &Actor::setNametag)
        .addFunction("getAttributesComponent", &Actor::getAttributesComponent)
        .addFunction("getMaxHealth", &Actor::getMaxHealth)
        .addFunction("getHealth", &Actor::getHealth)
        .addFunction("getAbsorption", &Actor::getAbsorption)
        .addFunction("getMaxAbsorption", &Actor::getMaxAbsorption)
        .addFunction("getAttribute", static_cast<AttributeInstance*(Actor::*)(AttributeId)>(&Actor::getAttribute))
        .addFunction("getAttribute", static_cast<AttributeInstance*(Actor::*)(int)>(&Actor::getAttribute))
        .addFunction("isOnFire", &Actor::isOnFire)
        .addFunction("getLocalName", &Actor::getLocalName)
        .addFunction("getPos", [](Actor* actor) -> glm::vec3
        {
            return *actor->getPos();
        })
        .addFunction("getPosPrev", [](Actor* actor) -> glm::vec3
        {
            return *actor->getPosPrev();
        })
        .addFunction("setPos", &Actor::setPos)
        .addFunction("getStateVectorComponent", &Actor::getStateVectorComponent)
        .addFunction("getMoveInputComponent", &Actor::getMoveInputComponent)
        .addFunction("getActorRotationComponent", &Actor::getActorRotationComponent)
        .addFunction("getAABBShapeComponent", &Actor::getAABBShapeComponent)
        .addFunction("getBlockMovementSlowdownMultiplierComponent", &Actor::getBlockMovementSlowdownMultiplierComponent)
        .addFunction("getActorHeadRotationComponent", &Actor::getActorHeadRotationComponent)
        .addFunction("getGameMode", &Actor::getGameMode)
        .addFunction("getLevel", &Actor::getLevel)
        .addFunction("getSupplies", &Actor::getSupplies)
        .addFunction("getSkin", &Actor::getSkin)
        .addFunction("getContainerManagerModel", &Actor::getContainerManagerModel)
        .endClass()
        .beginClass<GameMode>("GameMode")
        .addFunction("startDestroyBlock", &GameMode::startDestroyBlockByLua)
        .addFunction("destroyBlock", &GameMode::destroyBlock)
        .addFunction("continueDestroyBlock", &GameMode::continueDestroyBlockByLua)
        .addFunction("stopDestroyBlock", &GameMode::stopDestroyBlock)
        .addFunction("startBuildBlock", &GameMode::startBuildBlock)
        .addFunction("buildBlock", &GameMode::buildBlock)
        .addFunction("continueBuildBlock", &GameMode::continueBuildBlock)
        .addFunction("stopBuildBlock", &GameMode::stopBuildBlock)
        .addFunction("tick", &GameMode::tick)
        .addFunction("getPickRange", &GameMode::getPickRange)
        .addFunction("useItem", &GameMode::useItem)
        .addFunction("useItemOn", &GameMode::useItemOn)
        .addFunction("interact", &GameMode::interact)
        .addFunction("attack", &GameMode::attack)
        .addFunction("releaseUsingItem", &GameMode::releaseUsingItem)
        .addFunction("getDestroyRate", &GameMode::getDestroyRate)
        .addFunction("baseUseItem", &GameMode::baseUseItem)
        .addFunction("getDestroyProgress", [](GameMode* gameMode) -> float
        {
            return gameMode->mBreakProgress;
        })
        .endClass()
        /*class Level {
    public:
        CLASS_FIELD(uintptr_t**, mVfTable, 0x0);
        ///CLASS_FIELD(std::unordered_map<mce::UUID, PlayerListEntry>, pl, 0x1BC8); // class_field doesn't support std::unordered_map lol

        std::unordered_map<mce::UUID, PlayerListEntry>* getPlayerList();
        class HitResult* getHitResult();
        class SyncedPlayerMovementSettings* getPlayerMovementSettings();
        std::vector<Actor*> getRuntimeActorList();
    };*/
        .beginClass<AABB>("AABB")
        .addConstructor<void(*)(const glm::vec3&, const glm::vec3&)>()
        .addProperty("mMin", &AABB::mMin)
        .addProperty("mMax", &AABB::mMax)
        .endClass()
        .beginClass<Level>("Level")
        .addFunction("getPlayerList", &Level::getPlayerList)
        .addFunction("getHitResult", &Level::getHitResult)
        .addFunction("getPlayerMovementSettings", &Level::getPlayerMovementSettings)
        .addFunction("getRuntimeActorList", [](Level* level) -> std::vector<Actor*>
        {
            return level->getRuntimeActorList();
        })
        .endClass()
        .beginClass<std::vector<Actor*>>("ActorVector")
        .addFunction("size", &std::vector<Actor*>::size)
        .addFunction("get", [](std::vector<Actor*>* vec, size_t index) -> Actor*
        {
            if (index < 1 || index > vec->size())
            {
                return nullptr;
            }
            return (*vec)[index - 1];
        })
        .addFunction("__len", &std::vector<Actor*>::size)
        .addFunction("__index", [](std::vector<Actor*>* vec, size_t index) -> Actor*
        {
            if (index < 1 || index > vec->size())
            {
                return nullptr;
            }
            return (*vec)[index - 1];
        })
        .endClass()
    // std::vector<ImVec2>

        .beginClass<HitResult>("HitResult")
        .addProperty("mStartPos", &HitResult::mStartPos)
        .addProperty("mRayDir", &HitResult::mRayDir)
        .addFunction("getType", &HitResult::getType)
        .addFunction("getTypeString", &HitResult::getTypeString)
        .addProperty("mFacing", &HitResult::mFacing)
        .addProperty("mBlockPos", &HitResult::mBlockPos)
        .addProperty("mPos", &HitResult::mPos)
        .addProperty("mIsHitLiquid", &HitResult::mIsHitLiquid)
        .addProperty("mLiquidFacing", &HitResult::mLiquidFacing)
        .addProperty("mLiquid", &HitResult::mLiquid)
        .addProperty("mLiquidPos", &HitResult::mLiquidPos)
        .addProperty("mIndirectHit", &HitResult::mIndirectHit)
        .endClass()

        .beginClass<ActorRotationComponent>("ActorRotationComponent")
        .addProperty("mPitch", &ActorRotationComponent::mPitch)
        .addProperty("mYaw", &ActorRotationComponent::mYaw)
        .addProperty("mPitchOld", &ActorRotationComponent::mOldPitch)
        .addProperty("mYawOld", &ActorRotationComponent::mOldYaw)
        .endClass()
        .beginClass<StateVectorComponent>("StateVectorComponent")
        .addProperty("mPos", &StateVectorComponent::mPos)
        .addProperty("mPosOld", &StateVectorComponent::mPosOld)
        .addProperty("mVelocity", &StateVectorComponent::mVelocity)
        .endClass()
        .beginClass<ActorTypeComponent>("ActorTypeComponent")
        .addProperty("mType", &ActorTypeComponent::mType)
        .endClass()
        .beginClass<MoveInputComponent>("MoveInputComponent")
        .addFunction("getIsMoveLocked", [](MoveInputComponent* moveInputComponent) -> bool
        {
            return moveInputComponent->mIsMoveLocked;
        })
        .addFunction("getIsSneakDown", [](MoveInputComponent* moveInputComponent) -> bool
        {
            return moveInputComponent->mIsSneakDown;
        })
        .addFunction("getIsJumping", [](MoveInputComponent* moveInputComponent) -> bool
        {
            return moveInputComponent->mIsJumping;
        })
        .addFunction("getIsJumping2", [](MoveInputComponent* moveInputComponent) -> bool
        {
            return moveInputComponent->mIsJumping2;
        })
        .addFunction("getIsSprinting", [](MoveInputComponent* moveInputComponent) -> bool
        {
            return moveInputComponent->mIsSprinting;
        })
        .addFunction("getForward", [](MoveInputComponent* moveInputComponent) -> bool
        {
            return moveInputComponent->mForward;
        })
        .addFunction("getBackward", [](MoveInputComponent* moveInputComponent) -> bool
        {
            return moveInputComponent->mBackward;
        })
        .addFunction("getLeft", [](MoveInputComponent* moveInputComponent) -> bool
        {
            return moveInputComponent->mLeft;
        })
        .addFunction("getRight", [](MoveInputComponent* moveInputComponent) -> bool
        {
            return moveInputComponent->mRight;
        })
        .addFunction("getMoveVector", [](MoveInputComponent* moveInputComponent) -> glm::vec2
        {
            return moveInputComponent->mMoveVector;
        })
    // setters
        .addFunction("setIsMoveLocked", [](MoveInputComponent* moveInputComponent, bool value)
        {
            moveInputComponent->mIsMoveLocked = value;
        })
        .addFunction("setIsSneakDown", [](MoveInputComponent* moveInputComponent, bool value)
        {
            moveInputComponent->mIsSneakDown = value;
        })
        .addFunction("setIsJumping", [](MoveInputComponent* moveInputComponent, bool value)
        {
            moveInputComponent->mIsJumping = value;
        })
        .addFunction("setIsJumping2", [](MoveInputComponent* moveInputComponent, bool value)
        {
            moveInputComponent->mIsJumping2 = value;
        })
        .addFunction("setIsSprinting", [](MoveInputComponent* moveInputComponent, bool value)
        {
            moveInputComponent->mIsSprinting = value;
        })
        .addFunction("setForward", [](MoveInputComponent* moveInputComponent, bool value)
        {
            moveInputComponent->mForward = value;
        })
        .addFunction("setBackward", [](MoveInputComponent* moveInputComponent, bool value)
        {
            moveInputComponent->mBackward = value;
        })
        .addFunction("setLeft", [](MoveInputComponent* moveInputComponent, bool value)
        {
            moveInputComponent->mLeft = value;
        })
        .addFunction("setRight", [](MoveInputComponent* moveInputComponent, bool value)
        {
            moveInputComponent->mRight = value;
        })
        .addFunction("setMoveVector", [](MoveInputComponent* moveInputComponent, glm::vec2 value)
        {
            moveInputComponent->mMoveVector = value;
        })
        .endClass()
        .beginClass<AABBShapeComponent>("AABBShapeComponent")
        .addProperty("mHeight", &AABBShapeComponent::mHeight)
        .addProperty("mWidth", &AABBShapeComponent::mWidth)
        .addProperty("mMin", &AABBShapeComponent::mMin)
        .addProperty("mMax", &AABBShapeComponent::mMax)
        .endClass()
        .beginClass<BlockMovementSlowdownMultiplierComponent>("BlockMovementSlowdownMultiplierComponent")
        .addProperty("mMultiplier", &BlockMovementSlowdownMultiplierComponent::mBlockMovementSlowdownMultiplier)
        .endClass()
        .beginClass<ActorUtils>("ActorUtils")
        .addStaticFunction("getActorList", &ActorUtils::getActorList)
        .endClass()


    ;

    /*
    struct StateVectorComponent {
        glm::vec3 mPos;
        glm::vec3 mPosOld;
        glm::vec3 mVelocity;
    };
    Usage of StateVectorComponent in Lua:

    local stateVectorComponent = actor:getStateVectorComponent()
    local pos = stateVectorComponent.mPos
    */
}
