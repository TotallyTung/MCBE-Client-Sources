#include "Jetpack.h"
#include "../../../Client.h"
#include "../Exploit/AntiDesync.h"

Jetpack::Jetpack() : Module("Jetpack", "Fly in the direction you're looking.", Category::MOVEMENT) {
    slider<float>("Speed", "NULL", ValueType::FLOAT_T, &speedMod, 0.f, 3.f);
    boolsetting("Bypass", "NULL", &isBypass);
}

Jetpack::~Jetpack() {}

void Jetpack::onEnable() {}

void Jetpack::onNormalTick(Actor* actor) {
    static Killaura* kaMod = (Killaura*)client->moduleMgr->getModule("Killaura");
    static AntiDesync* AntiDesyncMod = (AntiDesync*)client->moduleMgr->getModule("AntiDesync");
    auto* localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return;
    Vec2<float>* rotation = localPlayer->getRotation();
    if (!rotation) return;
    float yaw = (kaMod->isEnabled() && kaMod->TargetDis < kaMod->MaxSrange && kaMod->TargetDis != 0) ? kaMod->rotAngle.y : rotation->y;
    float pitch = (kaMod->isEnabled() && kaMod->TargetDis < kaMod->MaxSrange && kaMod->TargetDis != 0) ? kaMod->rotAngle.x : rotation->x;
    if (AntiDesyncMod->isEnabled()) {
        if (mc.getLocalPlayer()->getItemUseDuration() > 0) yaw = rotation->y;
        if (mc.getLocalPlayer()->getItemUseDuration() > 0) pitch = rotation->x;
    }
    float calcYaw = (yaw + 90) * (PI / 180);
    float calcPitch = pitch * (PI / 180);
    Vec3<float> moveVec;
    moveVec.x = cos(calcYaw) * cos(calcPitch) * speedMod;
    moveVec.y = -sin(calcPitch) * speedMod;
    moveVec.z = sin(calcYaw) * cos(calcPitch) * speedMod;
    if (mc.isKeyDown(VK_SPACE)) {
        moveVec.y += 0.465f;
    }
    else if (mc.isKeyDown(VK_SHIFT)) {
        moveVec.y -= 0.465f;
    }
    localPlayer->lerpMotion(moveVec);
}
