#include "KillauraReach.h"
#include "../../Client/Client.h"
#include "../../../../../../SDK/Network/PacketSender.h"
Vec3<float> theOldPos;
KillauraReach::KillauraReach() : Module("KillauraReach", "Hit ur enemys Far (press F to hit)", Category::COMBAT) {
    registerSetting(new EnumSetting("Mode", "Attack mode type.", { "Press F", "Auto", "Left Click" }, &modeNZ, 0));
    registerSetting(new SliderSetting<float>("Speed", "teleport speed", &speed, 1.f, 1.f, 3.2f));
}

static __int64 ms;
static DWORD lastMS = GetTickCount();
static __int64 timeMS = -1;
static DWORD getCurrentMs() {
    return GetTickCount();
}

static __int64 getElapsedTime() {
    return getCurrentMs() - ms;
}

static void resetTime() {
    lastMS = getCurrentMs();
    timeMS = getCurrentMs();
}

static bool hasTimedElapsed(__int64 time, bool reset) {
    if (getCurrentMs() - lastMS > time) {
        if (reset)
            resetTime();
        return true;
    }
    return false;
}

void KillauraReach::onEnable() {
    valuePaipy = 0.0f;
    canMove = 0;  // Inicia com canMove desativado
}



void KillauraReach::onSendPacket(Packet* packet) {
    if (Game.getClientInstance()->getLocalPlayer() == nullptr) return;
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    theOldPos = Game.getLocalPlayer()->getPos();
    Minecraft* mc = Game.clientInstance->minecraft;
    Level* level = localPlayer->level;
    GameMode* gm = localPlayer->gameMode;
    if (level == nullptr) return;
    if (mc != nullptr) {
        *mc->minecraftTimer = 20.0f;
    }


    bool isFKeyPressed = (GetAsyncKeyState(0x46) & 0x8000) != 0; // 0x46 = 'F'

    // Atualiza o tempo e a condição do teleporte
    auto currentTime = std::chrono::steady_clock::now();
    if (modeNZ == 2) {


        if (Game.isLeftClickDown() && canMove == 0 && (currentTime - lastTeleportTime) >= minDelay) {
            if (!((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('A') & 0x8000) ||
                (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000) || (GetAsyncKeyState(VK_SHIFT) & 0x8000))) {
                canMove = 1;  // Inicia o teleporte
                lastTeleportTime = currentTime;
            }
        }
    }
    if (modeNZ == 0) {


        if (isFKeyPressed && canMove == 0 && (currentTime - lastTeleportTime) >= minDelay) {
            if (!((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('A') & 0x8000) ||
                (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000) || (GetAsyncKeyState(VK_SHIFT) & 0x8000))) {
                canMove = 1;  // Inicia o teleporte
                lastTeleportTime = currentTime;
            }
        }
    }
    if (modeNZ == 1) {


        if (canMove == 0 && (currentTime - lastTeleportTime) >= minDelay) {
            if (!((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState('A') & 0x8000) ||
                (GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000) || (GetAsyncKeyState(VK_SHIFT) & 0x8000))) {
                canMove = 1;  // Inicia o teleporte
                lastTeleportTime = currentTime;
            }
        }
    }

    // Sempre tenta encontrar o alvo mais próximo (mesmo fora do teleporte)
    Actor* closestTarget = nullptr;
    float minDistance = 300.0f; // Distância máxima de ataque
    for (auto& actor : level->getRuntimeActorList()) {
        if (TargetUtil::isFriendValid(actor)) {
            float distance = actor->getEyePos().dist(localPlayer->getEyePos());
            if (distance < minDistance) {
                minDistance = distance;
                closestTarget = actor;
            }
        }
    }

    // Se não houver alvo, não realiza ataque (você pode adicionar lógica extra aqui, se necessário)
    if (closestTarget == nullptr) {
        // Se não houver alvo, pode ser interessante resetar ou interromper o teleporte
        canMove = 0;
        *mc->minecraftTimer = 20.0f;
        return;
    }

    // Bloco responsável pelo teleporte (mantém a lógica original)
    if (canMove == 1) {
        //float distanceToClosestTarget = closestTarget->getEyePos().dist(localPlayer->getEyePos());
        float distanceToClosestTarget = localPlayer->getPos().dist(Vec3<float>(closestTarget->getPos().x, localPlayer->getPos().y, closestTarget->getPos().z));

        if (valuePaipy >= distanceToClosestTarget) {
            reverseTrick = 1;
        }
        else if (valuePaipy <= 0) {
            reverseTrick = 0;
        }

        if (reverseTrick == 1) {
            *mc->minecraftTimer = 999999;
            valuePaipy -= speed;
            if (valuePaipy <= 0) {
                reverseTrick = 0;
                canMove = 0;
                *mc->minecraftTimer = 20.0f;
            }
        }
        else {
            *mc->minecraftTimer = 999999;
            valuePaipy += speed;
            if (valuePaipy >= distanceToClosestTarget) {
                reverseTrick = 1;
                *mc->minecraftTimer = 20.0f;
            }
        }

        // Se quiser que o teleporte também realize os ataques dentro do seu fluxo, mantenha este código
        if (packet->getName() == "PlayerAuthInputPacket") {
            PlayerAuthInputPacket* paip = static_cast<PlayerAuthInputPacket*>(packet);
            // Calcula a direção do alvo e teleporta o jogador
            Vec3<float> direction = (closestTarget->getPos() - localPlayer->getPos()).normalize();
            paip->position.x += direction.x * valuePaipy;
            paip->position.z += direction.z * valuePaipy;
        }
    }

    // *** Bloco de ataque e swing fora da condição de teleporte ***
    // Esse bloco será executado sempre que um packet do tipo "PlayerAuthInputPacket" chegar, independente do teleporte.
    if (packet->getName() == "PlayerAuthInputPacket") {
        // Realiza o ataque e swing
        if (!ModuleManager::getModule<KillAura>()->isEnabled()) {
            localPlayer->swing();          // Swing
            gm->attack(closestTarget);       // Ataque
        }
    }

}


void KillauraReach::onDisable() {
    Minecraft* mc = Game.clientInstance->minecraft;
    *mc->minecraftTimer = 20.0f;
    Game.getLocalPlayer()->setPos(theOldPos);
}