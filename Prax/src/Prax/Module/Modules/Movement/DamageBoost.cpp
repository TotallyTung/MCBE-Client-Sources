//
// Created by vastrakai on 9/23/2023.
//

#include <Prax/SDK/Game/Network/Other/SetEntityMotionPacket.h>
#include <imgui_internal.h>
#include "DamageBoost.h"

void DamageBoost::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (!Misc::IsUsingMovementKeys()) return;
    if (!Minecraft::ClientInstance->ThePlayer()) return;

    if (packet->getId() == (int) PacketID::SetEntityMotion) {
        auto sem = (SetEntityMotionPacket*) packet.get();
        if (sem->RuntimeID == Minecraft::ClientInstance->ThePlayer()->getRuntimeIDComponent()->runtimeID) {
            auto player = Minecraft::ClientInstance->ThePlayer();
            auto velocity = player->GetStateVectorComponent()->velocity;
            auto appliedVelocity = sem->Motion;
            float appliedSpeed = sqrt(appliedVelocity.x * appliedVelocity.x + appliedVelocity.z * appliedVelocity.z);
            appliedSpeed *= 10.f;
            float speed = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

            float yaw = player->GetActorRotationComponent()->yaw + Misc::GetRotationKeyOffset();
            float x = -sinf(yaw * (float) IM_PI / 180.f);
            float z = cosf(yaw * (float) IM_PI / 180.f);
            float damageBoost = SpeedSetting.Value / 10;
            if (appliedSpeed > 1 && AdjustForKnockback.Enabled) damageBoost += appliedSpeed / 10;
            float vely = velocity.y;
            if (ApplyUpwardsVelocity.Enabled) vely = appliedVelocity.y;
            vely *= UpwardsMultiplier.Value;

            player->lerpMotion(Vector3(x * damageBoost, vely, z * damageBoost));
            *cancel = true;
        }
    }

}