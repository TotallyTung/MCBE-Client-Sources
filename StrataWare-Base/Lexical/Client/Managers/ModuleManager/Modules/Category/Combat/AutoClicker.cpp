#include "AutoClicker.h"

AutoClicker::AutoClicker() : Module("Clicker", "Automates clicking", Category::COMBAT) {
    registerSetting(new ToggleSetting("Right Click", "Automate right clicks", &rightclick, false));
    registerSetting(new ToggleSetting("Break Blocks", "Mine blocks automatically" , &breakBlocks, true));
    registerSetting(new NumberSetting<int>("Delay", "Delay between clicks (ticks)", &delay, 0, 0, 20));
    registerSetting(new ToggleSetting("Hold Mode", "Requires holding click", &hold, false));
}

void AutoClicker::onDisable() {
    resetState();
}

void AutoClicker::resetState() {
    Odelay = 0;
    consecutiveErrors = 0;
}
Actor* getActorFromEntityId2(EntityId entityId) {
    if (auto player = Game::ourPlayer()) {
        if (auto level = player->level) {
            for (auto actor : level->getRuntimeActorList()) {
                if (actor && actor->getEntityContext() &&
                    actor->getEntityContext()->mEntity.mRawId == entityId.mRawId) {
                    return actor;
                }
            }
        }
    }
    return nullptr;
}

void AutoClicker::onNormalTick(LocalPlayer* gm) {
    try {
        if (!isEnabled()) return;
        if (consecutiveErrors > 3 &&
            std::chrono::steady_clock::now() - lastErrorTime < std::chrono::seconds(1)) {
            return;
        }
        auto localPlayer = Game::ourPlayer();
        if (!localPlayer) return;
        if (Game::canUseMoveKeys()) {
            if (Game::isLeftClickDown() || !hold) {
                handleLeftClick(gm, localPlayer);
            }
            if (rightclick && (Game::isRightClickDown() || !hold)) {
                handleRightClick(gm, localPlayer);
            }
        }
    }
    catch (...) {
        consecutiveErrors++;
        lastErrorTime = std::chrono::steady_clock::now();
        if (consecutiveErrors > 5) {
            setEnabled(false);
        }
    }
}

void AutoClicker::handleLeftClick(LocalPlayer* gm, LocalPlayer* localPlayer) {
    if (++Odelay < delay) return;
    try {
        auto hitResult = localPlayer->level->getHitResult();
        if (!hitResult) {
            resetState();
            return;
        }
        localPlayer->swing();
        if (hitResult->type == HitResultType::ENTITY) {
            if (auto target = getActorFromEntityId2(hitResult->entity.id)) {
                safeAttack(gm, target);
            }
        }
        else if (breakBlocks) {
            safeBlockBreak(gm, hitResult->blockPos, hitResult->selectedFace);
        }
    }
    catch (...) {
        throw;
    }
    resetState();
}

void AutoClicker::handleRightClick(LocalPlayer* gm, LocalPlayer* localPlayer) {
    if (++Odelay < delay) return;
    try {
        auto hitResult = localPlayer->level->getHitResult();
        if (hitResult) {
            safeBlockPlace(gm, hitResult->blockPos, hitResult->selectedFace);
        }
    }
    catch (...) {
        throw;
    }
    resetState();
}

bool AutoClicker::safeAttack(LocalPlayer* gm, Actor* target) {
    if (!target || !gm) return false;
    if (target->getEntityContext()->mEntity.mRawId ==
        gm->getEntityContext()->mEntity.mRawId) {
        return false;
    }
    if (gm->gameMode) {
        gm->gameMode->attack(target);
        return true;
    }
    return false;
}

bool AutoClicker::safeBlockBreak(LocalPlayer* gm, const BlockPos& pos, uint8_t face) {
    if (!gm->gameMode) return false;
    bool isDestroyed = false;
    gm->gameMode->startDestroyBlock(pos, face, isDestroyed);
    gm->gameMode->stopDestroyBlock(pos);
    if (isDestroyed) {
        if (auto region = Game::getClientInstance()->getRegion()) {
            if (auto block = region->getBlock(pos)) {
                if (block->blockLegacy && block->blockLegacy->blockId != 0) {
                    gm->gameMode->destroyBlock(pos, 0);
                    return true;
                }
            }
        }
    }
    return false;
}

bool AutoClicker::safeBlockPlace(LocalPlayer* gm, const BlockPos& pos, uint8_t face) {
    if (!gm->gameMode) return false;
    bool result = true;
    gm->gameMode->buildBlock(pos, face, result);
    return result;
}
