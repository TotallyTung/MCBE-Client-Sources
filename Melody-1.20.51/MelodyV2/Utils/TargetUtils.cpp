// Created by Tony on 2024-10-10 10:01:54

#include "TargetUtils.h"
#include "../Utils/Math.h"
#include "../SDK/GameData.h"
#include "../Client/Client.h"
#include <string>

#include "../Client/ModuleManager/Modules/Player/AntiNigga.h"

bool TargetUtils::isTargetValid(Actor* target, bool isMob) {
    static AntiNigga* NiggaMod = (AntiNigga*)client->moduleMgr->getModule("AntiNigga");
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return false;
    if (!localPlayer->canAttack(target, false)) return false;
    if (target == nullptr) return false;
    if (target == localPlayer) return false;
    if (!target->isAlive()) return false;
    if (NiggaMod->isEnabled()) {
        if ((target->aabbComponent->height < 1.5f || target->aabbComponent->width < 0.49f || target->aabbComponent->height > 2.1f || target->aabbComponent->width > 0.9f)) return false;
        if (target->aabbComponent->width <= 0.01f || target->aabbComponent->height <= 0.01f) return false;
    }

    // Sanitize the target's name and check against the friends list
    std::string targetName = Utils::sanitize(*target->getNameTag());
    if (std::find(TargetUtils::Friend.begin(), TargetUtils::Friend.end(), targetName) != TargetUtils::Friend.end()) return false;

    if (!target->canShowNameTag()) return false;
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