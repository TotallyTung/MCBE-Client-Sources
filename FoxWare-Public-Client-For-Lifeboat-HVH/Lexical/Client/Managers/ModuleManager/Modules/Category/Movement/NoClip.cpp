#include "NoClip.h"

NoClip::NoClip() : Module("Noclip", "NoClip Fly", Category::MOVEMENT) {
    registerSetting(new SliderSetting<float>("Horizontal Speed", "Horizontal speed", &hSpeed, 1.f, 0.2f, 11.f));
    registerSetting(new SliderSetting<float>("Stuck Speed", "Horizontal speed", &stuck, 0.f, -3.0f, 5.f));
    registerSetting(new SliderSetting<float>("Vertical Speed", "Vertical speed", &vSpeed, 0.5f, 0.2f, 3.f));
    registerSetting(new SliderSetting<float>("Glide Value", "Value that how much u want to go down", &Glide, -0.02, -0.3, 0.f));
    registerSetting(new BoolSetting("Sm Border", "Avoid getting damage by lifeboat SM borer", &smborder, false));
    registerSetting(new BoolSetting("bypass", "Avoid getting damage by lifeboat SM borer", &bypass, false));
    registerSetting(new BoolSetting("hvhAirStuck", "Swape between 0 and HorizontalSpeed every Specified ticks", &hvhair, false));
}

float lerpNoClip(float endPoint, float current, float speed) {
    if (speed < 0.0) speed = 0.0;
    else if (speed > 1.0) speed = 1.0;

    float dif = std::max(endPoint, current) - std::min(endPoint, current);
    float factor = dif * speed;
    return current + (endPoint > current ? factor : -factor);
}

void NoClip::onNormalTick(LocalPlayer* localPlayer) {
    Vec3<float>& velocity = localPlayer->stateVector->velocity;
    velocity = Vec3<float>(0.f, 0.f, 0.f);
    if (hvhair) {
        tickCounter++; // Incrementa o contador

        // Alterna a velocidade a cada 1 tick
        if (tickCounter >= 1) {
            currentSpeed = (currentSpeed == stuck) ? hSpeed : stuck;
            tickCounter = 0; // Reseta o contador
        }
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

        if (isRight) moveValue.x += 1;
        if (isLeft) moveValue.x += -1;
        if (isForward) moveValue.y += 1;
        if (isBackward) moveValue.y += -1;

        // Movimentação vertical
        float verticalSpeed = 0.0f;
        if (isUp) verticalSpeed += vSpeed;
        if (isDown) verticalSpeed -= vSpeed;

        // Cálculo do ângulo do movimento
        float angleRad = (float)std::atan2(moveValue.x, moveValue.y);
        float angleDeg = angleRad * (180.f / PI);
        yaw += angleDeg;

        // Calculando o movimento
        Vec3<float> moveVec = Vec3<float>(0.f, verticalSpeed, 0.f);

        if ((moveValue.x != 0) || (moveValue.y != 0)) {
            float calcYaw = (yaw + 90.f) * (PI / 180.f);
            moveVec.x = cos(calcYaw) * currentSpeed;
            moveVec.z = sin(calcYaw) * currentSpeed;
        }

        // Aplicando limites de smborder no moveVec
        if (smborder) {
            float newX = localPlayer->stateVector->pos.x + moveVec.x;
            float newY = localPlayer->stateVector->pos.y + moveVec.y;
            float newZ = localPlayer->stateVector->pos.z + moveVec.z;

            if (newX < 41.0f) newX = 41.0f;
            if (newX > 1109.0f) newX = 1109.0f;
            if (newY > 161.0f) newY = 161.0f;
            if (newZ < 41.0f) newZ = 41.0f;
            if (newZ > 1109.0f) newZ = 1109.0f;

            // Atualiza o moveVec para garantir que o movimento fique dentro dos limites
            moveVec.x = newX - localPlayer->stateVector->pos.x;
            moveVec.y = newY - localPlayer->stateVector->pos.y;
            moveVec.z = newZ - localPlayer->stateVector->pos.z;
        }

        // Atualizando a posição do jogador diretamente
        Vec3<float> newPos;
        newPos.x = localPlayer->stateVector->pos.x + moveVec.x;
        newPos.y = localPlayer->stateVector->pos.y + moveVec.y;
        newPos.z = localPlayer->stateVector->pos.z + moveVec.z;
        localPlayer->setPos(newPos);  // Atualiza a posição com setPos
    }
}



void NoClip::onSendPacket(Packet* packet) {


    if ((packet->getName() == "PlayerAuthInputPacket" || packet->getName() == "MovePlayerPacket")) {
        auto* paip = (PlayerAuthInputPacket*)packet;
        auto* mpp = (MovePlayerPacket*)packet;
        static float offsetY = -0.3f;
        LocalPlayer* localPlayer = Game::getLocalPlayer();
        if (localPlayer == nullptr) {
            offsetY = -0.3f;
            return;
        }

        // Modificación para que el movimiento de subida y bajada sea de 0.02 y -0.02 cada 3 ticks
        if (paip) {
            if (bypass) {

                if (packet->getId() == PacketID::PlayerAuthInput) {
                    PlayerAuthInputPacket* paip = static_cast<PlayerAuthInputPacket*>(packet);
                    paip->position.y -= offsetY;
                    offsetY += 0.015f;
                    if (offsetY >= -0.281f) {
                        offsetY = -0.30;
                    }
                }
            }
        }
    }



}