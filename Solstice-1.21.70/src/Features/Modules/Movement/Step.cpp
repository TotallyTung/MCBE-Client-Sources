//
// Created by vastrakai on 8/3/2024.
//

#include "Step.hpp"

#include <Features/Events/BaseTickEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

void Step::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Step::onBaseTickEvent>(this);
}

void Step::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Step::onBaseTickEvent>(this);
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    player->getMaxAutoStepComponent()->mMaxStepHeight = 0.5625f;
}

bool mShouldClimb = false;
bool mShouldStop = false;

void Step::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    if (!player) return;

    bool isPressed = player->getMoveInputComponent()->mForward || player->getMoveInputComponent()->mBackward || player->getMoveInputComponent()->mLeft || player->getMoveInputComponent()->mRight;

    auto handleFlareonV2Step = [this, &player]() {
        if (player->isCollidingHorizontal() && player->getMoveInputComponent()->mForward) {
            player->getStateVectorComponent()->mVelocity.y = 0.0f;

            {
                auto& lowerPos = player->getAABBShapeComponent()->mMin;
                auto& upperPos = player->getAABBShapeComponent()->mMax;

                lowerPos.y += mStepHeight.mValue / 10;
                upperPos.y += mStepHeight.mValue / 10;
            }
        }
    };

    switch ((int)mMode.mValue) {
    case 0: // Vanilla
        player->getMaxAutoStepComponent()->mMaxStepHeight = mStepHeight.mValue;
        break;
    case 1: // Flareon
        if (isPressed && player->isCollidingHorizontal() && (player->isOnGround() || mShouldClimb)) {
            player->getStateVectorComponent()->mVelocity.y = mStepHeight.mValue / 10;
            mShouldStop = true;
            mShouldClimb = true;
        }
        else if (mShouldStop) {
            mShouldStop = false;
            mShouldClimb = false;
            player->getStateVectorComponent()->mVelocity.y = 0;
        }
        break;
    case 2: // Flareon V2
        if (isPressed && player->isCollidingHorizontal() && (player->isOnGround() || mShouldClimb)) {
            handleFlareonV2Step();

            mShouldStop = true;
            mShouldClimb = true;
        }
        else if (mShouldStop) {
            mShouldStop = false;
            mShouldClimb = false;

            player->getStateVectorComponent()->mVelocity.y = 0;
        }
        break;
    }
}
