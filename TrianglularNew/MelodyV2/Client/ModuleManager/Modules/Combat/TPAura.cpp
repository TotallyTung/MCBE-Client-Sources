#include "TpAura.h"
#include "../../../Client.h"
#include <cstdlib>
#include <cmath>
static std::vector<Actor*> targetListJ;
float lerpSpeed = 0.f;
bool attackMobs = false;
float horizontalOffset = 2.0f; 
int tpmode = 0;
int multipliera = 0;
float angle = 0.0f;
bool attack1 = false;
TpAura::TpAura() : Module("TpAura", "Constantly teleports & attacks enemies.", Category::COMBAT) {
    slider<int>("Range", "Teleport range", ValueType::INT_T, &range, 3, 40);
    slider<int>("Y Offset", "Adjust teleport height", ValueType::INT_T, &yOffset, -5, 10);
    slider<int>("Attack Multiplier", "How many times to attack", ValueType::INT_T, &multipliera, 0, 10);
    slider<float>("Lerp Speed", "Interpolation speed", ValueType::FLOAT_T, &lerpSpeed, 0.1f, 1.0f);
    slider<float>("Horizontal Offset", "Random horizontal position offset", ValueType::FLOAT_T, &horizontalOffset, 0.f, 5.0f);
    boolsetting("Attack Mobs", "Enable attacking mobs", &attackMobs);
    boolsetting("Attack", "Enable attacking", &attack1);
    enumsetting("TPMode", "NULL", { "Random", "Circle+" }, &tpmode);
}

void TpAura::onEnable() {
    currentTarget = nullptr;
    angle = 0.0f;
}

void TpAura::findEntityhhD() {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return;
    auto level = localPlayer->getLevel();
    if (!level) return;
    targetListJ.clear();

    for (auto& target : level->getRuntimeActorList()) {
        if (!attackMobs) {
            if (!target->isPlayer() || target == localPlayer) continue;
        }
        if (!TargetUtils::isTargetValid(target, attackMobs)) continue;
        float dist = target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
        if (dist < range) {
            targetListJ.push_back(target);
        }
    }
}

void TpAura::onNormalTick(Actor* actor) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return;
    auto level = localPlayer->getLevel();
    if (!level) return;

    findEntityhhD();
    if (targetListJ.empty()) return;

    currentTarget = targetListJ[0];
    if (!currentTarget) return;

    targetPos = *currentTarget->getPosition();
    targetPos.y = currentTarget->stateVectorComponent->pos.y + yOffset;

    Vec3 offsetTargetPos = targetPos;

    if (tpmode == 0) {
        float randomOffsetX = ((std::rand() % 2001) - 1000) / 1000.0f * horizontalOffset;
        float randomOffsetZ = ((std::rand() % 2001) - 1000) / 1000.0f * horizontalOffset;
        offsetTargetPos.x += randomOffsetX;
        offsetTargetPos.z += randomOffsetZ;
    }
    else if (tpmode == 1) {
        angle += 0.1f; 
        offsetTargetPos.x += horizontalOffset * cos(angle);
        offsetTargetPos.z += horizontalOffset * sin(angle);
    }
    if (tpmode == 0) {
        Vec3 currentPos = localPlayer->stateVectorComponent->pos;
        Vec3 interpolatedPos = currentPos.lerp(offsetTargetPos, lerpSpeed);
        localPlayer->lerpMotion(interpolatedPos - currentPos);
    }
    if (tpmode == 1) {
        Vec3 currentPos1 = localPlayer->stateVectorComponent->pos;
        Vec3 interpolatedPos1 = currentPos1.lerp(offsetTargetPos, lerpSpeed * 2);
        localPlayer->lerpMotion(interpolatedPos1 - currentPos1);
    }
    if (attack1) {
        for (auto i = 0; i < multipliera; i++) {
            mc.getGameMode()->attack(currentTarget);
            localPlayer->swing();
        }
    }
}

void TpAura::onDisable() {
    currentTarget = nullptr;
}

