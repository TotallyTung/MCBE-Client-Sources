#include "Killaura.h"
#include "../../../../../Client.h"
#include <algorithm> 
float hookminrange;
int targeting = 0; 
Killaura::Killaura() : Module("Killaura-CAT", "attacks entities around you", Category::COMBAT) {
    registerSetting(new BoolSetting("Mobs", "attack mobs", &mobs, false));
    registerSetting(new SliderSetting<float>("Range", "range in which targets will be hit", &range, 3.5f, 1.f, 50.f));
    registerSetting(new SliderSetting<float>("Hook Min Range", "range in which targets will be hit", &hookminrange, 3.5f, 1.f, 15.f));
    registerSetting(new BoolSetting("useHook", "rotate", &useHookspeed, true));
    registerSetting(new SliderSetting<float>("HookSpeed", "edits rotate and fly speed based off factor", &hookspeedH, 3.5f, 1.f, 50.f));
    registerSetting(new SliderSetting<int>("Attempts", "amount of attacks", &hitattemptsFE, 3, 0, 50));
    registerSetting(new BoolSetting("Rotations", "rotate", &rotate, true));
    registerSetting(new SliderSetting<int>("Delay", "delay", &delayFE, 3, 0, 20));
    registerSetting(new EnumSetting("Target", "Target filter", { "Distance", "Health" }, &targeting, 0));
}

void Killaura::onDisable() {
    delayOFE = 0;
    arewehooking = false;
}

void Killaura::onEnable() {
    delayOFE = 0;
    arewehooking = false;
}

void Killaura::onNormalTick(LocalPlayer* localPlayer) {
    if (!localPlayer) return;
    if (delayOFE++ >= delayFE) {
        delayOFE = 0;
        std::vector<Actor*> validTargets;
        for (Actor* ent : localPlayer->getlevel()->getRuntimeActorList()) {
            if (!TargetUtil::isTargetValid(ent, mobs)) continue;
            if (localPlayer->getPos().dist(ent->getPos()) > range) continue;
            validTargets.push_back(ent);
        }
        if (targeting == 0) {
            std::sort(validTargets.begin(), validTargets.end(), [localPlayer](Actor* a, Actor* b) {
                return localPlayer->getPos().dist(a->getPos()) < localPlayer->getPos().dist(b->getPos());
                });
        }
        else if (targeting == 1) {
            std::sort(validTargets.begin(), validTargets.end(), [](Actor* a, Actor* b) {
                return a->getHealth() > b->getHealth();
                });
        }
        if (!validTargets.empty()) {
            Actor* target = validTargets.front();
            int attacks = hitattemptsFE;
            for (int i = 0; i < attacks; i++) {
                localPlayer->getgameMode()->attack(target);
                if (HitResult* hitResult = localPlayer->level->getHitResult()) {
                    hitResult->type = HitResultType::ENTITY;
                }
                localPlayer->swing();
            }
        }
    }
}

void Killaura::onUpdateRotation(LocalPlayer* localPlayer) {
    if (!rotate || !localPlayer) return;
    BlockSource* region = Game::clientInstance->getRegion();
    Level* level = localPlayer->level;
    bool foundTarget = false;
    std::vector<Actor*> validTargets;
    for (Actor* ent : localPlayer->getlevel()->getRuntimeActorList()) {
        if (!TargetUtil::isTargetValid(ent, mobs)) continue;
        float distance = localPlayer->getPos().dist(ent->getPos());
        if (distance > range) continue;
        validTargets.push_back(ent);
    }
    if (targeting == 0) {
        std::sort(validTargets.begin(), validTargets.end(), [localPlayer](Actor* a, Actor* b) {
            return localPlayer->getPos().dist(a->getPos()) < localPlayer->getPos().dist(b->getPos());
            });
    }
    else if (targeting == 1) {
        std::sort(validTargets.begin(), validTargets.end(), [](Actor* a, Actor* b) {
            return a->getHealth() > b->getHealth();
            });
    }
    if (!validTargets.empty()) {
        Actor* target = validTargets.front();
        float distance = localPlayer->getPos().dist(target->getPos());
        Vec2<float> targetRot = localPlayer->getEyePos().CalcAngle(target->getEyePos());
        float targetYaw = targetRot.y;
        if (auto* headRot = localPlayer->getActorHeadRotationComponent()) {
            headRot->headYaw = targetYaw;
        }
        if (auto* bodyRot = localPlayer->getMobBodyRotationComponent()) {
            bodyRot->bodyYaw = targetYaw;
        }
        localPlayer->rotation->presentRot = targetRot;
        if (distance < hookminrange && useHookspeed) 
            arewehooking = true;
        }
        else {
            arewehooking = false;
        }
        foundTarget = true;
    if (!foundTarget) {
        arewehooking = false;
    }
}

void Killaura::onSendPacket(Packet* packet) {
}
