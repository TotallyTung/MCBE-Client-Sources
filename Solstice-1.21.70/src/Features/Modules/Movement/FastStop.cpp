//
// Created by vastrakai on 8/3/2024.
//

#include "FastStop.hpp"

#include <Features/Events/BaseTickEvent.hpp>

void FastStop::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &FastStop::onBaseTickEvent>(this);
}

void FastStop::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &FastStop::onBaseTickEvent>(this);
}

void FastStop::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    if (!player) return;

    mWasUsingMoveKeys = mIsUsingMoveKeys;
    mIsUsingMoveKeys = Keyboard::isUsingMoveKeys();

    if (mWasUsingMoveKeys && !mIsUsingMoveKeys)
    {
        player->getStateVectorComponent()->mVelocity.x = 0;
        player->getStateVectorComponent()->mVelocity.z = 0;
    }
}
