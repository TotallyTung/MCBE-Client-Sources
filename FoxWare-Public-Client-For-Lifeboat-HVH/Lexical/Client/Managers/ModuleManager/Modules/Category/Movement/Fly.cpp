#include "Fly.h"

Fly::Fly() : Module("Fly", "Fly like Superman, defy gravity and move through the air", Category::MOVEMENT) {
    registerSetting(new SliderSetting<float>("Horizontal Speed", "Controls your horizontal flying speed", &hSpeed, 1.f, 0.2f, 15.f));
    registerSetting(new SliderSetting<float>("Switch Speed", "Sets the speed", &stuck, 0.f, -3.0f, 9.f));
    registerSetting(new SliderSetting<float>("Vertical Speed", "Controls your vertical flying speed", &vSpeed, 0.5f, 0.2f, 3.f));
    registerSetting(new SliderSetting<float>("Vertical Offset", "Adjust the Y-axis offset to fine-tune your position", &Yset, 0.5f, -6.0f, 6.f));
    registerSetting(new SliderSetting<float>("Glide Rate", "Adjusts how much you descend while gliding", &Glide, -0.02, -0.3, 0.f));
    registerSetting(new BoolSetting("Avoid Border Damage", "Prevents damage from Lifeboat's SM border", &smborder, false));
    registerSetting(new BoolSetting("Bypass", "Bypasses Lifeboat's SM border to avoid damage", &bypass, false));
    registerSetting(new BoolSetting("Switch", "Switches between switch speed and Horizontal Speed", &hvhair, false));
}


float lerpfly(float endPoint, float current, float speed) {
    if (speed < 0.0) speed = 0.0;
    else if (speed > 1.0) speed = 1.0;

    float dif = std::max(endPoint, current) - std::min(endPoint, current);
    float factor = dif * speed;
    return current + (endPoint > current ? factor : -factor);
}
#include "../Combat/KillAura.h"
#include "../../../ModuleManager.h"
void Fly::onNormalTick(LocalPlayer* localPlayer) {
    if (hvhair) {
        tickCounter++;
        if (tickCounter >= 1) {
            currentSpeed = (currentSpeed == stuck) ? hSpeed : stuck;
            tickCounter = 0;
        }
    }
    else {
        currentSpeed = hSpeed;
    }
    Vec3<float>& velocity = localPlayer->stateVector->velocity;
    velocity = Vec3<float>(0.f, 0.f, 0.f);

    velocity.y += Glide;
    static Killaura* kamod = ModuleManager::getModule<Killaura>();
    if (kamod->useHookspeed && kamod->arewehooking) {
        currentSpeed = kamod->hookspeedH;
    }
    else {
        currentSpeed = hSpeed;
    }
    if (Game::canUseMoveKeys()) {
        float yaw = localPlayer->rotation->presentRot.y;

        bool isForward = Game::isKeyDown('W');
        bool isLeft = Game::isKeyDown('A');
        bool isBackward = Game::isKeyDown('S');
        bool isRight = Game::isKeyDown('D');
        bool isUp = Game::isKeyDown(VK_SPACE);
        bool isDown = Game::isKeyDown(VK_SHIFT);

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
            velocity.y += vSpeed;

        if (isDown)
            velocity.y -= vSpeed;

        float angleRad = (float)std::atan2(moveValue.x, moveValue.y);
        float angleDeg = angleRad * (180.f / PI);
        yaw += angleDeg;
        Vec3<float> moveVec = Vec3<float>(0.f, velocity.y, 0.f);
        if ((moveValue.x != 0) || (moveValue.y != 0)) {
            float calcYaw = (yaw + 90.f) * (PI / 180.f);
            moveVec.x = cos(calcYaw) * currentSpeed;
            moveVec.z = sin(calcYaw) * currentSpeed;
        }
        if (smborder) {
            float newX = localPlayer->stateVector->pos.x + moveVec.x;
            float newY = localPlayer->stateVector->pos.y + moveVec.y;
            float newZ = localPlayer->stateVector->pos.z + moveVec.z;
            if (newX < 41.0f) newX = 41.0f;
            if (newX > 1109.0f) newX = 1109.0f;
            if (newY > 161.0f) newY = 161.0f;
            if (newZ < 41.0f) newZ = 41.0f;
            if (newZ > 1109.0f) newZ = 1109.0f;
            moveVec.x = newX - localPlayer->stateVector->pos.x;
            moveVec.y = newY - localPlayer->stateVector->pos.y;
            moveVec.z = newZ - localPlayer->stateVector->pos.z;
        }
        localPlayer->lerpMotion(moveVec);
    }
}


void Fly::onSendPacket(Packet* packet) {
    if ((packet->getName() == "PlayerAuthInputPacket" || packet->getName() == "MovePlayerPacket")) {
        auto* paip = (PlayerAuthInputPacket*)packet;
        auto* mpp = (MovePlayerPacket*)packet;
        static float offsetY = -0.3f;
        LocalPlayer* localPlayer = Game::getLocalPlayer();
        if (localPlayer == nullptr) {
            offsetY = -0.3f;
            return;
        }
        if (paip) {
            if (bypass) {
                if (packet->getId() == PacketID::PlayerAuthInput) {
                    PlayerAuthInputPacket* paip = static_cast<PlayerAuthInputPacket*>(packet);
                    paip->position.y -= offsetY;
                    paip->position.y += Yset;
                    offsetY += 0.015f;
                    if (offsetY >= -0.281f) {
                        offsetY = -0.30;
                    }
                }
            }
        }
    }
}