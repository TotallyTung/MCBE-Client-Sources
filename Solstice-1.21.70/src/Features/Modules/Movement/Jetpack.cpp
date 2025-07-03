//
// Created by alteik on 15/10/2024.
//

#include "Jetpack.hpp"
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>

void Jetpack::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Jetpack::onBaseTickEvent>(this);
}

void Jetpack::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Jetpack::onBaseTickEvent>(this);

    if(mReset.mValue)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if(!player) return;
        player->getStateVectorComponent()->mVelocity = {0, 0, 0};
    }
}

void Jetpack::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if(!player) return;

    float calcYaw = (player->getActorRotationComponent()->mYaw + 90) * (PI / 180);
    float calcPitch = (player->getActorRotationComponent()->mPitch) * -(PI / 180);

    glm::vec3 moveVec;
    moveVec.x = cos(calcYaw) * cos(calcPitch) * mHorizontalSpeed.mValue;
    moveVec.y = sin(calcPitch) * mVerticalSpeed.mValue;
    moveVec.z = sin(calcYaw) * cos(calcPitch) * mHorizontalSpeed.mValue;

    player->getStateVectorComponent()->mVelocity = moveVec;
}

