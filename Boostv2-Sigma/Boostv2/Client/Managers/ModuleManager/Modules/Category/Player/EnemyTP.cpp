#include "EnemyTp.h"
static bool sasas = false;
const int MIN_X = 60;
const int MAX_X = 1090;
const int MIN_Z = 60;
const int MAX_Z = 1090;

EnemyTp::EnemyTp() : Module("EnemyTp", "Teleport to the nearest player with a certain speed.", Category::PLAYER) {
    registerSetting(new SliderSetting<int>("Range", "NULL", &range, 60, 3, 300));
}

bool EnemyTp::sortByDist(Actor* a1, Actor* a2) {
    Vec3<float> lpPos = Game.getLocalPlayer()->getPos();
    return ((a1->getPos().dist(lpPos)) < (a2->getPos().dist(lpPos)));
}

void EnemyTp::onEnable() {
    if (sasas == false) {
        sasas = true;
    }
    else {
        sasas = false;
    }
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    GameMode* gm = localPlayer->gameMode;
    Minecraft* mc = Game.clientInstance->minecraft;

    if (!localPlayer) return;

    Level* level = localPlayer->level;
    if (!level) return;

    BlockSource* region = Game.clientInstance->getRegion();
    targetListTP.clear();

    // Inicializa valores corretamente
    works = false;
    tpdone = false;
    closestDistance = std::numeric_limits<float>::max();
    goal = Vec3<float>(0, 0, 0);

    for (auto& entity : level->getRuntimeActorList()) {
        if (TargetUtil::isTargetValid(entity)) {
            float dist = localPlayer->getPos().dist(entity->getPos());
            if (dist <= range && dist < closestDistance) {
                closestDistance = dist;
                goal = entity->getPos();
            }
        }
    }





    if (closestDistance < std::numeric_limits<float>::max()) {
        works = true;
        Game.DisplayClientMessage("%s[%sEnemyTp%s] %sPlayer found!", MCTF::WHITE, MCTF::RED, MCTF::WHITE, MCTF::WHITE);
    }
    else {
        Game.DisplayClientMessage("%s[%sEnemyTp%s] %sCouldn't find any players within range%s!", MCTF::WHITE, MCTF::RED, MCTF::WHITE, MCTF::WHITE, MCTF::RED);
        Minecraft* mc = Game.clientInstance->minecraft;
        *mc->minecraftTimer = static_cast<float>(20);
        this->setEnabled(false);
    }
}

void EnemyTp::onNormalTick(LocalPlayer* actor) {
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (!localPlayer || !works || tpdone) return;

    Vec3<float> currentPos = localPlayer->getPos();
    float distance = currentPos.dist(goal);

    if (distance <= 1.0f) {
        localPlayer->setPos(goal);
        this->setEnabled(false);
        return;
    }

    float lerpFactor = 0.2f;
    if (distance <= 13.0f) lerpFactor = 0.35f;
    else if (distance <= 20.0f) lerpFactor = 0.35f;
    else if (distance <= 100.0f) lerpFactor = 0.35f;
    else lerpFactor = 0.075f;

    Vec3<float> newPos = currentPos.lerpTo(goal, lerpFactor);
    localPlayer->setPos(newPos);

}
void EnemyTp::onSendPacket(Packet* packet) {

}

void EnemyTp::onDisable() {
    Minecraft* mc = Game.clientInstance->minecraft;
    *mc->minecraftTimer = 20.0f;
}