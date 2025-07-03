#include "TpCommand.h"
#include "../../../Client.h"
#include <cmath>

TpCommand::TpCommand() : CommandBase("tp", "Teleport to specified coordinates", "<x> <y> <z>", { "teleport" }) {
}

bool TpCommand::execute(const std::vector<std::string>& args) {
    if (args.size() < 4) {
        return false;
    }

    float targetX = std::stof(args[1]);
    float targetY = std::stof(args[2]);
    float targetZ = std::stof(args[3]);

    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (!localPlayer) {
        Client::DisplayClientMessage("%sPlayer not found!", MCTF::RED);
        return true;
    }

    Vec3<float> currentPos = localPlayer->getPos();
    Vec3<float> targetPos(targetX, targetY, targetZ);

    // If we're already close enough to the target position, just teleport there
    float distance = currentPos.dist(targetPos);
    if (distance <= 1.0f) {
        localPlayer->setPos(targetPos);
        Client::DisplayClientMessage("%sTeleported instantly to [%s%.2f%s, %s%.2f%s, %s%.2f%s]", MCTF::WHITE, MCTF::GRAY, targetPos.x, MCTF::RESET, MCTF::GRAY, targetPos.y, MCTF::RESET, MCTF::GRAY, targetPos.z, MCTF::RESET);
        return true;
    }

    // Start teleporting smoothly using lerp logic
    float lerpFactor = 0.2f; // Slow lerp at first

    if (distance <= 13.0f) lerpFactor = 0.55f;
    else if (distance <= 20.0f) lerpFactor = 0.45f;
    else if (distance <= 100.0f) lerpFactor = 0.10f;
    else lerpFactor = 0.075f;

    // Teleport over time
    localPlayer->setPos(currentPos.lerpTo(targetPos, lerpFactor));

    Client::DisplayClientMessage("%sTeleporting to [%s%.2f%s, %s%.2f%s, %s%.2f%s]...", MCTF::WHITE, MCTF::GRAY, targetPos.x, MCTF::RESET, MCTF::GRAY, targetPos.y, MCTF::RESET, MCTF::GRAY, targetPos.z, MCTF::RESET);

    return true;
}
