//
// Created by vastrakai on 7/18/2024.
//

#include "SafeWalk.hpp"

#include <Features/Events/BaseTickEvent.hpp>

DEFINE_NOP_PATCH_FUNC(patchSafeWalk, SigManager::SneakMovementSystem_tickSneakMovementSystem, 2);
DEFINE_NOP_PATCH_FUNC(patchSafeWalkSneak, SigManager::SneakMovementSystem_tickSneakMovementSystem+2, 3);


void SafeWalk::onEnable()
{
    patchSafeWalk(true);
    patchSafeWalkSneak(true);
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &SafeWalk::onBaseTickEvent>(this);
}

void SafeWalk::onDisable()
{
    patchSafeWalk(false);
    patchSafeWalkSneak(false);
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &SafeWalk::onBaseTickEvent>(this);
}

void SafeWalk::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    patchSafeWalk(mEnabled);
    patchSafeWalkSneak(!player->getMoveInputComponent()->mIsSneakDown && mEnabled);
}
