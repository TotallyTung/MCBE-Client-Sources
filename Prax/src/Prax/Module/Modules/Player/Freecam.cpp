//
// Created by vastrakai on 10/21/2023.
//

#include <Prax/Utils/System/KeyManager.h>
#include <Prax/Utils/Math.h>
#include "Freecam.h"

#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>

AABB Freecam::LastAABB = AABB();
Vector3 Freecam::LastPos = Vector3();
Vector3 Freecam::LastOldPos = Vector3();
Vector3 Freecam::LastVel = Vector3();
Vector2 Freecam::LastYaw = Vector2();
Vector2 Freecam::LastPitch = Vector2();
Vector2 Freecam::LastHeadRot = Vector2();
Vector2 Freecam::LastBodyRot = Vector2();

bool wasLagbacked = false;


void Freecam::OnEnable() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    LastAABB = player->GetAABB();
    LastPos = player->GetStateVectorComponent()->pos;
    LastOldPos = player->GetStateVectorComponent()->posOld;
    LastVel = player->GetStateVectorComponent()->velocity;
    auto actorRotations = player->GetActorRotationComponent();
    auto actorHeadRotations = player->GetActorHeadRotationComponent();
    auto mobBodyRot = player->entityCtx.getComponent<MobBodyRotationComponent>();

    float realYaw = actorRotations->yaw;
    float realOldYaw = actorRotations->oldYaw;
    float realOldPitch = actorRotations->oldPitch;
    float realPitch = actorRotations->pitch;
    float realHeadRot = actorHeadRotations->headRot;
    float realOldHeadRot = actorHeadRotations->oldHeadRot;
    float realBodyYaw = mobBodyRot->yBodyRot;
    float realOldBodyYaw = mobBodyRot->yOldBodyRot;

    LastYaw = Vector2(realYaw, realOldYaw);
    LastPitch = Vector2(realPitch, realOldPitch);
    LastHeadRot = Vector2(realHeadRot, realOldHeadRot);
    LastBodyRot = Vector2(realBodyYaw, realOldBodyYaw);

}

void Freecam::OnDisable() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (wasLagbacked) {
        wasLagbacked = false;
        return;
    }

    player->SetAABB(LastAABB);
    player->GetStateVectorComponent()->pos = LastPos;
    player->GetStateVectorComponent()->posOld = LastOldPos;
    player->GetStateVectorComponent()->velocity = LastVel;
}

void Freecam::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (Flight.Enabled) {
        Vector2 motion = Misc::GetMotion(player->GetActorRotationComponent()->yaw, Speed.Value);
        bool space = KeyManager::Keys[VK_SPACE];
        bool shift = KeyManager::Keys[VK_SHIFT];

        float y = 0;

        if (space) {
            y = Speed.Value / 10;
        } else if (shift) {
            y = -Speed.Value / 10;
        } else {
            y = 0.f;
        }

        player->GetStateVectorComponent()->velocity = Vector3(motion.x, y, motion.y);
    }
}

void Freecam::OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    if (packet->getId() == (int)PacketID::MovePlayer) *canceled = true;
    if (packet->getId() == (int)PacketID::PlayerAuthInput) *canceled = true;
}

void Freecam::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int)PacketID::MovePlayer)
    {
        /*
        // Follow the lagback and set the original position
        if (!player) return;

        player->GetStateVectorComponent()->pos = mpp->position;
        player->GetStateVectorComponent()->posOld = mpp->position;
        AABB aabb = player->GetAABBForPos(mpp->position);
        LastAABB = aabb;
        */

        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        auto mpp = std::reinterpret_pointer_cast<MovePlayerPacket>(packet);
        if (mpp->actorRuntimeID != player->GetRuntimeID()) return;



        wasLagbacked = true;
        if (DisableOnLagback.Enabled) {
            if(Enabled) Toggle();
        }

    }
}

void Freecam::OnRender() {
    // Draw box with an orange color
    if (RenderLastPos.Enabled) {
        Math::DrawBoxFilled(LastAABB.lower - Vector3(0.1f, 0.1f, 0.1f), LastAABB.upper + Vector3(0.1f, 0.1f, 0.1f), 1.0f, ImColor(255, 165, 0, 255 / 2),
                            ImColor(255, 165, 0, 255 / 2));
    }
}