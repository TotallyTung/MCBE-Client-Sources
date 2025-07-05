//
// Created by vastrakai on 10/13/2023.
//

#include <Prax/SDK/Game/Entity/Components/BlockMovementSlowdownMultiplierComponent.h>
#include "NoSlowDown.h"

void NoSlowDown::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    //mp->setFrictionModifier(1.0f);
    player->entityCtx.getComponent<BlockMovementSlowdownMultiplierComponent>()->blockMovementSlowdownMultiplier = Vector3(0.f, 0.f, 0.f);
}

void NoSlowDown::OnEnable() {

}

void NoSlowDown::OnDisable() {

}
