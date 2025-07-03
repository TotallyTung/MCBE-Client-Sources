//
// Created by ssi on 8/17/2024.
//

#include "Spider.hpp"
#include <Features/Events/BaseTickEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

void Spider::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Spider::onBaseTickEvent>(this);
}

void Spider::onBaseTickEvent(BaseTickEvent& event) {
    const auto player = event.mActor;
    if (!player) return;
    if (mOnGroundOnly.mValue && !player->isOnGround()) return;

    if (mMode.mValue == Mode::Clip && player->isCollidingHorizontal() && player->getMoveInputComponent()->mForward) {
        const auto state = player->getStateVectorComponent();
        const auto aabbShape = player->getAABBShapeComponent();

        state->mVelocity.y = 0.f;

        aabbShape->mMin.y += mSpeed.mValue;
        aabbShape->mMax.y += mSpeed.mValue;
    }

#ifdef __PRIVATE_BUILD__

    else if (mMode.mValue == Mode::Flareon) {

        auto state = player->getStateVectorComponent();
        auto playerPos = *player->getPos();

        if (!player->isCollidingHorizontal() || !player->getMoveInputComponent()->mForward) {
            mPosY = 0.f;
            if (!player->isCollidingHorizontal() && mWasCollided) {
                mWasCollided = false;
                state->mVelocity.y = 0.05f;
            }
            return;
        }

        mWasCollided = true;

        if (mPosY == 0.f) {
            mPosY = playerPos.y;
            state->mVelocity.y = 0.f;
        }
        auto dist = playerPos.y - mPosY;
        if (dist < 1.3f) {
            state->mVelocity.y = mSpeed.mValue;
        }
        else {
            state->mVelocity.y = -(mSpeed.mValue / 10.f);
            auto pos = playerPos;
            pos.y -= 0.06f;
            mPosY = pos.y;
        }
    }

#endif

}

void Spider::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Spider::onBaseTickEvent>(this);
#ifdef __PRIVATE_BUILD__
    if(mMode.mValue == Mode::Flareon)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if(!player) return;
        const auto state = player->getStateVectorComponent();
        state->mVelocity.y = 0.f;
    }
#endif
}