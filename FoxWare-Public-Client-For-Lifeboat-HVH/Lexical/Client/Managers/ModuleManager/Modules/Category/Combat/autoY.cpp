#include "AutoY.h"
#include "../../../../../Client.h"
#include <algorithm>  
int yoffset;
float yspeedh;
AutoY::AutoY() : Module("AutoY-CAT", "attacks entities around you", Category::COMBAT) {
    registerSetting(new BoolSetting("Mobs", "attack mobs", &mobs, false));
    registerSetting(new SliderSetting<float>("Range", "range in which targets will be hit", &range, 9.5f, 0.f, 15.f));
    registerSetting(new SliderSetting<int>("Y-offset", "y offset", &yoffset, -7, -10, 10));
    registerSetting(new SliderSetting<float>("Y-speed", "y offset", &yspeedh, 0.f, 0.f, 5.f));
}

void AutoY::onDisable() {
}

void AutoY::onEnable() {
}
#include "../../../../../Client.h"
void AutoY::onNormalTick(LocalPlayer* localPlayer) {
    if (!localPlayer) return;
    static float currentYOffset = 0.f;
    const float interpolationSpeed = yspeedh;
    bool foundTarget = false;
    for (Actor* ent : localPlayer->getlevel()->getRuntimeActorList()) {
        if (!TargetUtil::isTargetValid(ent, mobs)) continue;
        if (localPlayer->getPos().dist(ent->getPos()) > range) continue;
        foundTarget = true;
        break;
    }
    if (foundTarget) {
        if (yoffset > currentYOffset) {
            currentYOffset += interpolationSpeed;
            if (currentYOffset > yoffset) currentYOffset = yoffset;
        }
        else if (yoffset < currentYOffset) {
            currentYOffset -= interpolationSpeed;
            if (currentYOffset < yoffset) currentYOffset = yoffset;
        }
    }
    else {
        if (currentYOffset > 0.f) {
            currentYOffset -= interpolationSpeed;
            if (currentYOffset < 0.f) currentYOffset = 0.f;
        }
        else if (currentYOffset < 0.f) {
            currentYOffset += interpolationSpeed;
            if (currentYOffset > 0.f) currentYOffset = 0.f;
        }
    }
    if (currentYOffset != 0.f) {
        Vec3<float> currentPos = localPlayer->getPos();
        localPlayer->setPos(Vec3<float>(currentPos.x, currentPos.y + currentYOffset, currentPos.z));
    }
}
void AutoY::onUpdateRotation(LocalPlayer* localPlayer) {
}

void AutoY::onSendPacket(Packet* packet) {
}
