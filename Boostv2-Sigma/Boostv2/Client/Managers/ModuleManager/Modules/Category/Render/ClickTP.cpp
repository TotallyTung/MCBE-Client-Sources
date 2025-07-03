#include "ClickTP.h"
#include <limits>
#include <Windows.h>   // Para GetAsyncKeyState

ClickTP::ClickTP() : Module("ClickTP", "TP to the block lol gg 10hp", Category::WORLD) {
    oldRenderAABB.lower.y = -999.f;
    works = true;         // O módulo permanece sempre ativo.
    tpdone = false;
    isTeleporting = false;  // Variável para controlar se o teleporte está ocorrendo.
}

void ClickTP::onLevelRender() {
    BlockSource* region = Game.clientInstance->getRegion();
    if (region == nullptr)
        return;

    Level* level = Game.getLocalPlayer()->level;
    if (level == nullptr)
        return;

    HitResult* hitResult = level->getHitResult();
    if (hitResult == nullptr)
        return;

    // Se o hitResult for de um bloco, captura a posição com o botão direito.
    if (hitResult->type == HitResultType::BLOCK) {
        // Detecta se o botão direito foi pressionado (somente na transição de não pressionado para pressionado)
        static bool wasRightPressed = false;
        bool rightPressed = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
        if (rightPressed && !wasRightPressed) {
            // Captura a posição do bloco destacado
            BlockPos blockLookingAt = hitResult->blockPos;
            this->highlightBlockPos = blockLookingAt;
            // Atualiza o destino do teleporte (centralizado e acima do bloco)
            goal = Vec3<float>(highlightBlockPos.x + 0.5f,
                highlightBlockPos.y + 1.0f,
                highlightBlockPos.z + 0.5f);
            // Exibe as mensagens informando a captura
            Game.DisplayClientMessage("Position Set: X: %.2f Y: %.2f Z: %.2f",
                (float)highlightBlockPos.x, (float)highlightBlockPos.y, (float)highlightBlockPos.z);
            Game.DisplayClientMessage("Press Shift to teleport!");
        }
        wasRightPressed = rightPressed;

        // Renderiza a caixa de destaque ao redor do bloco
        BlockPos blockLookingAt = hitResult->blockPos;
        Block* block = region->getBlock(blockLookingAt);
        AABB blockAABB;
        block->blockLegacy->getOutline(block, region, &blockLookingAt, &blockAABB);

        AABB renderAABB = blockAABB;
        if (slide) {
            if (oldRenderAABB.lower.y == -999.f) {
                oldRenderAABB = renderAABB;
            }
            else {
                float animSpeed = MCR::deltaTime * slideSpeed;
                oldRenderAABB.lower = oldRenderAABB.lower.lerp(blockAABB.lower, animSpeed, animSpeed, animSpeed);
                oldRenderAABB.upper = oldRenderAABB.upper.lerp(blockAABB.upper, animSpeed, animSpeed, animSpeed);
            }
            renderAABB = oldRenderAABB;
        }
        else {
            oldRenderAABB.lower.y = -999.f;
        }
        MCR::drawBox3dFilled(renderAABB, UIColor(color.r, color.g, color.b, alpha),
            UIColor(color.r, color.g, color.b, lineAlpha));
    }
    else {
        oldRenderAABB.lower.y = -999.f;
    }
}

bool ClickTP::sortByDist(Actor* a1, Actor* a2) {
    Vec3<float> lpPos = Game.getLocalPlayer()->getPos();
    return ((a1->getPos().dist(lpPos)) < (a2->getPos().dist(lpPos)));
}

void ClickTP::onEnable() {
    // O módulo permanece ativo sem ações imediatas.
    works = true;
    tpdone = false;
    // Ao ativar, garante o timer padrão.
    Minecraft* mc = Game.clientInstance->minecraft;
    *mc->minecraftTimer = 20.0f;
}

void ClickTP::onNormalTick(LocalPlayer* actor) {
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (!localPlayer || !works)
        return;

    Minecraft* mc = Game.clientInstance->minecraft;

    // Detecta a borda de subida do Shift para iniciar o teleporte.
    static bool wasShiftPressed = false;
    bool currentShift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    if (!isTeleporting && currentShift && !wasShiftPressed) {
        isTeleporting = true;
        // Enquanto o teleporte ocorrer, define o timer para timerV.
        *mc->minecraftTimer = static_cast<float>(timerV);
    }
    wasShiftPressed = currentShift;

    // Se estiver teleportando, mover o jogador até o destino usando a lógica de lerpfactor.
    if (isTeleporting) {
        Vec3<float> currentPos = localPlayer->getPos();
        float distance = currentPos.dist(goal);
        float lerpFactor = 0.0f;

        if (distance <= 0.25f) {
            lerpFactor = 0.0f; // Para quando estiver muito perto do destino
            *mc->minecraftTimer = 20.0f;
            isTeleporting = false;  // Finaliza o teleporte
            Game.DisplayClientMessage("Teleported!");
            return;
        }
        else if (distance <= 13.0f) {
            lerpFactor = 0.55f;
        }
        else if (distance <= 15.0f) {
            lerpFactor = 0.45f;
        }
        else if (distance <= 20.0f) {
            lerpFactor = 0.45f;
        }
        else if (distance <= 50.0f) {
            lerpFactor = 0.25f;
        }
        else if (distance <= 100.0f) {
            lerpFactor = 0.10f;
        }
        else if (distance <= 140.0f) {
            lerpFactor = 0.10f;
        }
        else if (distance <= 170.0f) {
            lerpFactor = 0.10f;
        }
        else if (distance <= 200.0f) {
            lerpFactor = 0.10f;
        }
        else if (distance <= 250.0f) {
            lerpFactor = 0.075f;
        }
        else if (distance <= 300.0f) {
            lerpFactor = 0.075f;
        }

        Vec3<float> newPos = currentPos.lerpTo(goal.add2(0, 1, 0), lerpFactor);
        localPlayer->setPos(newPos);
        Vec3<float>& velocity = localPlayer->stateVector->velocity;
        velocity = Vec3<float>(0.f, 0.f, 0.f);
    }
    else {
        // Se não estiver teleportando, assegura o timer padrão.
        *mc->minecraftTimer = 20.0f;
    }
}

void ClickTP::onDisable() {
    Minecraft* mc = Game.clientInstance->minecraft;
    *mc->minecraftTimer = 20.0f;
}
