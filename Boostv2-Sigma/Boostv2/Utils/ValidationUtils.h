#pragma once
#include "../SDK/World/Actor.h"
#include "../SDK/World/LocalPlayer.h"
#include "../SDK/World/Level/BlockSource.h"
#include "../SDK/World/Level/Level.h"

class ValidationUtils {
public:
    static bool isValid(Actor* actor) {
        return actor != nullptr && actor->isAlive();
    }
    static bool isValid(LocalPlayer* player) {
        return player != nullptr
            && player->isAlive()
            && player->level != nullptr;
    }
    static bool isValid(Level* level) {
        return level != nullptr;
    }
    static bool isValid(BlockSource* region) {
        return region != nullptr;
    }
    static bool isValidContext(LocalPlayer* player, Level* level = nullptr, BlockSource* region = nullptr) {
        return isValid(player)
            && (level ? isValid(level) : true)
            && (region ? isValid(region) : true);
    }
};