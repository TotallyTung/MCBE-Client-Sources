//
// Created by vastrakai on 8/24/2023.
//

#include <Prax/SDK/Game/Network/Other/SetEntityMotionPacket.h>
#include "Velocity.h"

void Velocity::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (!Minecraft::ClientInstance->ThePlayer()) return;
    if (packet->getId() == (int)PacketID::SetEntityMotion) {
        auto sem = (SetEntityMotionPacket*)packet.get();
        if (sem->RuntimeID == Minecraft::ClientInstance->ThePlayer()->getRuntimeIDComponent()->runtimeID) {
            Vector3 motion = sem->Motion;
            float heightMul = Vertical.Value;
            float horizontalMul = Horizontal.Value;
            motion.x *= horizontalMul;
            motion.z *= horizontalMul;
            motion.y *= heightMul;
            sem->Motion = motion;
            if (motion.x == 0 && motion.y == 0 && motion.z == 0) *cancel = true;
        }
    }
}