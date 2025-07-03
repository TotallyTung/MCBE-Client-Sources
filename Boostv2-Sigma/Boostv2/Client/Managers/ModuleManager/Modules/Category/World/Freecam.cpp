#include "Freecam.h"
Vec3<float> thetppos;
Freecam::Freecam() : Module("FreeCam", "Move your camera without moving the player.", Category::WORLD) {
    registerSetting(new SliderSetting<float>("Speed", "NULL", &speed, 1.f, 0.50f, 1.25f));
}

Freecam::~Freecam() {
}

void Freecam::onEnable() {
    auto Player = Game.getLocalPlayer();
    if (Player != nullptr) {
        initialViewAngles = Vec2(Player->rotation->presentRot.x, Player->rotation->presentRot.y);
        oldPos = Player->getPos(); 
    }
}

void Freecam::onDisable() {
    auto Player = Game.getLocalPlayer();
    if (Player != nullptr) {
            Player->setPos(oldPos);
    }
}

void Freecam::onMCRender(MinecraftUIRenderContext* rcx) {
    auto Player = Game.getLocalPlayer();
    auto pos = Game.getLocalPlayer()->getPos();
    if (Player == nullptr) return;

    Vec3<float>& velocity = Player->stateVector->velocity;
    velocity = Vec3<float>(0.f, 0.f, 0.f);

    if (Game.canUseMoveKeys()) {
        float yaw = Player->rotation->presentRot.y;

        bool isForward = Game.isKeyDown('W');
        bool isLeft = Game.isKeyDown('A');
        bool isBackward = Game.isKeyDown('S');
        bool isRight = Game.isKeyDown('D');
        bool isUp = Game.isKeyDown(VK_SPACE);
        bool isDown = Game.isKeyDown(VK_SHIFT);

        Vec2<int> moveValue;

        if (isRight)
            moveValue.x += 1;
        if (isLeft)
            moveValue.x += -1;
        if (isForward)
            moveValue.y += 1;
        if (isBackward)
            moveValue.y += -1;

        if (isUp)
            velocity.y += speed * 0.50;
        if (isDown)
            velocity.y -= speed* 0.50;

        float angleRad = (float)std::atan2(moveValue.x, moveValue.y);
        float angleDeg = angleRad * (180.f / PI);
        yaw += angleDeg;

        Vec3<float> moveVec = Vec3<float>(0.f, velocity.y, 0.f);

        if ((moveValue.x != 0) || (moveValue.y != 0)) {
            float calcYaw = (yaw + 90.f) * (PI / 180.f);
            moveVec.x = cos(calcYaw) * speed;
            moveVec.z = sin(calcYaw) * speed;
        }

        pos.x += moveVec.x;
        pos.y += moveVec.y;
        pos.z += moveVec.z;
        Player->setPos(pos);
        pos = thetppos;
    }
}