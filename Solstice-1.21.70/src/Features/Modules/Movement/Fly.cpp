//
// Created by vastrakai on 6/30/2024.
//

#include "Fly.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/Components/StateVectorComponent.hpp>
#include <SDK/Minecraft/Actor/Components/ActorRotationComponent.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <SDK/Minecraft/Network/Packets/SetActorMotionPacket.hpp>

class PlayerAuthInputPacket;

void Fly::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Fly::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Fly::onPacketOutEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (mMode.mValue == Mode::Vertical) {
        if (!player->isOnGround()) {
            displayDebug("Failed to teleport");
            setEnabled(false);
            return;
        }
        glm::ivec3 playerPos = *player->getPos();
        bool foundBlock = false;
        glm::ivec3 top = { 0, 0, 0 };
        float floorY = floorf(playerPos.y);
        for (int i = 0; i < 50; i++) {
            glm::ivec3 blockPos = { playerPos.x, floorY + i, playerPos.z };
            if (!BlockUtils::isAirBlock(blockPos)) {
                top = blockPos;
                foundBlock = true;
            }
        }
        if (foundBlock) {
            topY = top.y + 1;
        }
        else {
            displayDebug("Failed to teleport");
            setEnabled(false);
        }
        return;
    }

    // return if da mode isnt jump
    if (mMode.mValue != Mode::Jump) return;

    mCurrentY = player->getPos()->y;
    mLastJump = NOW;
    displayDebug("Jump fly enabled");
}

void Fly::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Fly::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Fly::onPacketOutEvent>(this);
    if (mTimerBoost.mValue) ClientInstance::get()->getMinecraftSim()->setSimTimer(20);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    AABBShapeComponent* aabb = player->getAABBShapeComponent();
    if (aabb->mMax.y != aabb->mMin.y + aabb->mHeight) {
        aabb->mMax.y = aabb->mMin.y + aabb->mHeight;
    }

    if (mMode.mValue != Mode::Jump) return;

    displayDebug("Jump fly disabled");
}

void Fly::displayDebug(const std::string& message) const
{
    if (mDebug.mValue)
    {
        ChatUtils::displayClientMessageSub("§6Fly", message);
        spdlog::debug("[Fly] {}", message);
    }
}

void Fly::onBaseTickEvent(BaseTickEvent& event)
{
    static bool setLast = false;

    bool applyTimer = true;

    auto player = event.mActor;
    if (mMode.mValue == Mode::Motion || mMode.mValue == Mode::Elytra)
    {
        glm::vec3 motion = glm::vec3(0, 0, 0);

        if (Keyboard::isUsingMoveKeys(true))
        {
            glm::vec2 calc = MathUtils::getMotion(player->getActorRotationComponent()->mYaw, mSpeed.mValue / 10);
            motion.x = calc.x;
            motion.z = calc.y;

            bool isJumping = player->getMoveInputComponent()->mIsJumping;
            bool isSneaking = player->getMoveInputComponent()->mIsSneakDown;

            if (isJumping)
                motion.y += mSpeed.mValue / 10;
            else if (isSneaking)
                motion.y -= mSpeed.mValue / 10;
        }

        player->getStateVectorComponent()->mVelocity = motion;
    }
    else if (mMode.mValue == Mode::Pregame)
    {
        glm::vec3 motion = glm::vec3(0, -0.0005f, 0);

        if (Keyboard::isUsingMoveKeys(true))
        {
            float speed = 0.255;
            glm::vec2 calc = MathUtils::getMotion(player->getActorRotationComponent()->mYaw, speed);
            motion.x = calc.x;
            motion.z = calc.y;

            bool isJumping = player->getMoveInputComponent()->mIsJumping;
            bool isSneaking = player->getMoveInputComponent()->mIsSneakDown;

            if (isJumping)
                motion.y += speed;
            else if (isSneaking)
                motion.y -= speed;
        }

        player->getStateVectorComponent()->mVelocity = motion;
    }
    else if (mMode.mValue == Mode::Jump)
    {
        applyTimer = tickJump(player);
    }
    else if (mMode.mValue == Mode::Vertical) {
        const auto state = player->getStateVectorComponent();
        const auto aabbShape = player->getAABBShapeComponent();

        state->mVelocity = { 0.f, 0.f, 0.f };

        aabbShape->mMin.y += mStep.mValue;
        aabbShape->mMax.y = aabbShape->mMin.y;

        if (topY <= aabbShape->mMin.y) {
            displayDebug("Teleported");
            setEnabled(false);
            return;
        }
    }

    if (mTimerBoost.mValue && applyTimer)
    {
        setLast = true;
        ClientInstance::get()->getMinecraftSim()->setSimTimer(mTimerBoostValue.mValue);
    } else if (!mTimerBoost.mValue && setLast || !applyTimer)
    {
        setLast = false;
        ClientInstance::get()->getMinecraftSim()->setSimTimer(20);
    }
}

bool Fly::tickJump(Actor* player)
{
    static int flyTicks = 0;
    static bool wasFlying = false;

    if (mDamageOnly.mValue && mLastDamage < NOW)
    {
        if (wasFlying)
        {
            mCurrentY = player->getPos()->y;
            if (mResetOnDisable.mValue)
                player->getStateVectorComponent()->mVelocity = glm::vec3(0.f, player->getStateVectorComponent()->mVelocity.y, 0.f);

            wasFlying = false;
        }

        mCurrentFriction = 1.f;
        flyTicks = 0;
        return false;
    }

    wasFlying = true;

    if (player->isOnGround())
    {
        displayDebug("Resetting friction [OnGround]");
        mCurrentFriction = 1.f;
    }
    else if (mSpeedFriction.mValue)
    {
        mCurrentFriction *= mFriction.mValue;

        glm::vec2 motion = MathUtils::getMotion(player->getActorRotationComponent()->mYaw, (mSpeed.mValue * mCurrentFriction) / 10);
        player->getStateVectorComponent()->mVelocity = glm::vec3(motion.x, player->getStateVectorComponent()->mVelocity.y, motion.y);
    }

    if (player->getPos()->y < mCurrentY && !player->isOnGround())
    {
        if (NOW - mLastJump > static_cast<uint64_t>(mJumpDelay.mValue) * 1000)
        {
            jump();
            mLastJump = NOW;
        }

        displayDebug("Height: " + std::to_string(player->getPos()->y - mCurrentY));
        mCurrentY -= mHeightLoss.mValue;
    }

    if (player->isOnGround() && mResetOnGround.mValue)
    {
        mCurrentY = player->getPos()->y;
        displayDebug("Resetting height [OnGround]");
    }

    flyTicks++;
    return true; // tells da parent func to apply timer
}

void Fly::jump()
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (player == nullptr)
        return;

    bool onGround = player->isOnGround();
    player->setOnGround(true);
    player->jumpFromGround();
    player->setOnGround(onGround);

    glm::vec2 motion = MathUtils::getMotion(player->getActorRotationComponent()->mYaw, mSpeed.mValue / 10);
    player->getStateVectorComponent()->mVelocity = glm::vec3(motion.x, player->getStateVectorComponent()->mVelocity.y, motion.y);

    displayDebug("Jumping");
    mCurrentFriction = 1.f;
    displayDebug("Resetting friction [Jump]");
}

void Fly::onPacketOutEvent(PacketOutEvent& event) const
{

    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if (player == nullptr)
            return;

        auto packet = event.getPacket<PlayerAuthInputPacket>();
        if (mMode.mValue == Mode::Motion && mApplyGlideFlags.mValue)
        {
            packet->mInputData |= AuthInputAction::START_GLIDING;
            packet->mInputData &= ~AuthInputAction::STOP_GLIDING;
        }
        if (mMode.mValue == Mode::Elytra)
        {
            static bool alternating = false;
            alternating = !alternating;
            packet->mInputData |= AuthInputAction::START_GLIDING | AuthInputAction::ASCEND | AuthInputAction::WANT_UP | AuthInputAction::STOP_GLIDING;

            if (alternating)
                packet->mInputData |= AuthInputAction::JUMPING | AuthInputAction::START_JUMPING | AuthInputAction::JUMP_DOWN;
            packet->mInputData &= ~AuthInputAction::DESCEND | AuthInputAction::WANT_DOWN | AuthInputAction::SNEAKING | AuthInputAction::SNEAK_TOGGLE_DOWN | AuthInputAction::START_SNEAKING;
        }
    }
}

void Fly::onPacketInEvent(PacketInEvent& event)
{
    if (mMode.mValue  != Mode::Jump) return;

    if (event.mPacket->getId() == PacketID::SetActorMotion)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        auto sem = event.getPacket<SetActorMotionPacket>();
        if (sem->mRuntimeID == player->getRuntimeID())
        {
            mLastDamage = NOW + static_cast<uintptr_t>(mFlyTime.mValue) * 1000;
            mCurrentY = player->getPos()->y;
            if (mEnabled) displayDebug("Damage taken");
        }
    }

}
