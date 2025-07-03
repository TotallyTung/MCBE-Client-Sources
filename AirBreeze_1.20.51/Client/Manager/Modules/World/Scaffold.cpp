#include "Scaffold.h"
#include "../../Manager.h"
#include "../../Client.h"
#include "../../../Include/imgui/imgui.h"
#include <cmath>
#include <chrono>

void Scaffold::onBaseTick(Actor* actor, Level* level, GameMode* gm) {
    auto instance = Minecraft::getClientInstance();
    auto localPlayer = instance->getLocalPlayer();

    if (!localPlayer) return;

    auto Pos = localPlayer->getPosition();

    gm->buildBlock(Vec3<int>(Pos->x, Pos->y - 3, Pos->z), 0, true);
}