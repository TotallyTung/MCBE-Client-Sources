//
// Created by dark on 9/19/2024.
//

#include "TargetStrafe.hpp"
#include "Speed.hpp"

#include <Features/Modules/Combat/Aura.hpp>

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/RenderEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/KeyboardMouseSettings.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>

void TargetStrafe::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &TargetStrafe::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<RenderEvent, &TargetStrafe::onRenderEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &TargetStrafe::onPacketOutEvent>(this);
}

void TargetStrafe::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &TargetStrafe::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<RenderEvent, &TargetStrafe::onRenderEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &TargetStrafe::onPacketOutEvent>(this);

    mShouldStrafe = false;

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    handleKeyInput(false, false, false, false);
}

void TargetStrafe::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    glm::vec3 playerPos = *player->getPos();
    auto moveInputComponent = player->getMoveInputComponent();
    static auto speed = gFeatureManager->mModuleManager->getModule<Speed>();
    auto& keyboard = *ClientInstance::get()->getKeyboardSettings();
    bool isJumping = Keyboard::mPressedKeys[keyboard["key.jump"]];


    if (!Aura::sHasTarget || !Aura::sTarget || !Aura::sTarget->getActorTypeComponent() || (mJumpOnly.mValue && !isJumping) || (mSpeedOnly.mValue && (speed == nullptr || !speed->mEnabled)))
    {
        mShouldStrafe = false;
        return;
    }

    if (mSpeedOnly.mValue && mJumpOnly.mValue) moveInputComponent->mIsJumping = false;

    mCurrentTarget = Aura::sTarget;
    glm::vec3 targetPos = *mCurrentTarget->getPos();
    float dist = glm::distance(glm::vec2(playerPos.x, playerPos.z), glm::vec2(targetPos.x, targetPos.z));
    mForward = mDistance.mValue < dist;
    mBackward = dist < mMinDistance.mValue;

    if (mWallCheck.mValue && player->isCollidingHorizontal()) {
        mMoveRight = !mMoveRight;
    }
    else {
        if (moveInputComponent->mLeft) mMoveRight = false;
        else if (moveInputComponent->mRight) mMoveRight = true;
    }

    handleKeyInput(mForward, !mMoveRight, mBackward, mMoveRight);
    mShouldStrafe = true;
}

void TargetStrafe::onRenderEvent(RenderEvent& event) 
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (mShouldStrafe)
    {
        if (!mCurrentTarget->getActorTypeComponent()) return;
        float yaw = MathUtils::getRots(*player->getPos(), mCurrentTarget->getAABB()).y;
        auto rotationComponent = player->getActorRotationComponent();
        rotationComponent->mYaw = yaw;
        rotationComponent->mOldYaw = yaw;
        handleKeyInput(mForward, !mMoveRight, mBackward, mMoveRight);
    }
}

void TargetStrafe::onPacketOutEvent(PacketOutEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto pkt = event.getPacket<PlayerAuthInputPacket>();
        if (mShouldStrafe) {
            if (mAlwaysSprint.mValue) {
                pkt->mInputData |= AuthInputAction::SPRINT_DOWN | AuthInputAction::SPRINTING | AuthInputAction::START_SPRINTING;
                pkt->mInputData &= ~AuthInputAction::STOP_SPRINTING;

                if (mMoveRight) pkt->mMove = { 0.45f, 0.45f };
                else pkt->mMove = { 0.45f, -0.45f };                
            }
        }
    }
}

void TargetStrafe::handleKeyInput(bool pressingW, bool pressingA, bool pressingS, bool pressingD)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    auto moveInputComponent = player->getMoveInputComponent();

    moveInputComponent->mForward = pressingW;
    moveInputComponent->mLeft = pressingA;
    moveInputComponent->mBackward = pressingS;
    moveInputComponent->mRight = pressingD;
}