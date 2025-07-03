#include "ElytraFly.h"
static float Speedy = 4.0f;
ElytraFly::ElytraFly() : Module("ElytraFly", "Extend reach.", Category::MOVEMENT) {
    registerSetting(new KeybindSetting("Up Key", "Key for up", &upKey, 0));
    registerSetting(new KeybindSetting("Down Key", "Key for down", &downKey, 0));
    registerSetting(new SliderSetting<float>("X-Axis", "NULL", &Speed, 25.f, 0.f, 50.f));
    registerSetting(new SliderSetting<float>("Y-Axis", "NULL", &Speedy, 25.f, 0.f, 50.f));
}
Vec2<float> getKotion(float speed) {
    float playerYaw = Game::getLocalPlayer()->rotation->prevRot.y;
    bool w = Game::isKeyDown('W');
    bool a = Game::isKeyDown('A');
    bool s = Game::isKeyDown('S');
    bool d = Game::isKeyDown('D');
    if (w) {
        if (!a && !d) playerYaw += 90.0f;
        if (a) playerYaw += 45.0f;
        else if (d) playerYaw += 135.0f;
    }
    else if (s) {
        if (!a && !d) playerYaw -= 90.0f;
        if (a) playerYaw -= 45.0f;
        else if (d) playerYaw -= 135.0f;
    }
    else {
        if (!a && d) playerYaw += 180.0f;
    }
    float calcYaw = playerYaw * 0.01745329251f; //0.01745329251f = PI / 180
    return Vec2<float>(cos(calcYaw) * speed / 10.0f, sin(calcYaw) * speed / 10.0f);
}
bool isZoving(bool jumpCheck) {
    if (!Game::canUseMoveKeys()) return false;
    if (Game::isKeyDown('W') || Game::isKeyDown('A') || Game::isKeyDown('S') || Game::isKeyDown('D')) return true;
    if (jumpCheck && Game::isKeyDown(VK_SPACE)) return true;
    return false;
}

void setpseeed(float speed) {
    Vec2<float> motion = getKotion(speed);
    if (!isZoving(false)) {
        motion.x = 0.f;
        motion.y = 0.f;
    }
    Game::getLocalPlayer()->stateVector->velocity.x = motion.x;
    Game::getLocalPlayer()->stateVector->velocity.z = motion.y;
}

void ElytraFly::onNormalTick(LocalPlayer* localPlayer) {
    float flySpeed = Speed;
    bool hasElytraInSlot = localPlayer->getArmor(1)->item && localPlayer->getArmor(1)->item->texture_name == "elytra";  // Elytra check
    bool isRightSize = localPlayer->aabbShape->aabb.size.y <= 0.6f;
    if (isRightSize && hasElytraInSlot) {
        setpseeed(flySpeed);
        if (Game::isKeyDown(upKey)) localPlayer->stateVector->velocity.y = Speedy / 10.0f;
        else if (Game::isKeyDown(downKey)) localPlayer->stateVector->velocity.y = -Speedy / 10.0f;
        else localPlayer->stateVector->velocity.y = 0.05f; // counteract glide
    }
}