#include "Airjump.h"

#include <Windows.h>
#include "../../../../SDK/Classes/Actor.h"
#include "../../../Client.h"
#include "../../../../SDK/Components/Flags/OnGroundFlag.h"
Airjump::Airjump() : Module("AirJump", "Jump At Air", Category::MOVEMENT) {
}

void Airjump::onNormalTick(Actor* actor) {
    auto localPlayer = mc.getLocalPlayer();

    if (mc.getLocalPlayer() == nullptr) return;
     mc.getLocalPlayer()->isOnGround();
}
