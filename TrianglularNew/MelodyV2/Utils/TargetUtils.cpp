#include "TargetUtils.h"
#include "../Utils/Math.h"
#include "../SDK/GameData.h"
#include "../Client/Client.h"
#include <string>


bool TargetUtils::isTargetValid(Actor* target, bool isMob) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return false;
    if (!localPlayer->canAttack(target, false)) return false;
    if (target == nullptr) return false;
    if (target == localPlayer) return false;
    if (!target->isAlive()) return false;

    // Sanitize the target's name and check against the friends list
    std::string targetName = Utils::sanitize(*target->getNameTag());
    if (std::find(TargetUtils::Friend.begin(), TargetUtils::Friend.end(), targetName) != TargetUtils::Friend.end()) return false;


    const int entId = target->getEntityTypeId();

    // List of invalid entity IDs
    const std::vector<int> invalidEntityIds = {
        0,     // unknown
        64,    // item
        66,    // falling block
        69,    // xp orb
        70,    // enderEye
        95,    // falling block
        4194372, // exp bottle
        4194390, // potion
        4194405, // still potion
        4194391, // ender pearl
        12582992 // arrow???
    };

    if (std::find(invalidEntityIds.begin(), invalidEntityIds.end(), entId) != invalidEntityIds.end()) return false;

    if (!isMob) {
        if (!target->isPlayer()) return false;
        if (entId != 319) return false; // Ensure it's a player
        if (!target->canShowNameTag()) return false;

    }
    else {
        // Additional checks for mobs can be added here
    }

    return true;
}

bool TargetUtils::sortByDist(Actor* a1, Actor* a2) {
	Vec3<float> lpPos = *mc.getLocalPlayer()->getPosition();
	return ((a1->getPosition()->dist(lpPos)) < (a2->getPosition()->dist(lpPos)));
}