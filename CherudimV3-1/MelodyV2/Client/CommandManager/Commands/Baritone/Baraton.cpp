#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../Command.h"
#include "../../CommandManager.h"
#include "../../../Client.h"

Baritone::Baritone() : Command("Baritone", "Automation", { "Bari", "Bot", "Automation" }, "<u got some stupititys>") {}
# define M_PI 3.14159265358979323846 /* pi */

Vec2<float> gEtRotations(Vec3<float> playerEyePos, Vec3<float> targetPos) {
    Vec3<float> delta = targetPos.sub(playerEyePos);
    float yaw = atan2(delta.z, delta.x) * 180.0f / M_PI;
    float pitch = atan2(delta.y, sqrt(delta.x * delta.x + delta.z * delta.z)) * 180.0f / M_PI;
    return { -pitch, yaw - 90 };
}
void Baritone::execute(const std::vector<std::string>& args) {
    if (args.size() < 4 || args[1] != "walk") {
        char errorMsg[] = "[%sMelody%s] %sInvalid command! Usage: .Baritone walk <x> <y> <z>";
        mc.DisplayClientMessage(errorMsg, DARK_PURPLE, WHITE, RED);
        return;
    }

   // mc.getLocalPlayer()->stateVectorComponent->velocity = { 0, 0.1f, 0 };
}
