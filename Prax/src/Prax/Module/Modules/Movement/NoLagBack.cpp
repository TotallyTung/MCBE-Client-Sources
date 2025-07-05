//
// Created by vastrakai on 8/31/2023.
//

#include "NoLagBack.h"
#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>

void NoLagBack::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {

    if (packet->getId() == (int)PacketID::MovePlayer) {
        auto mpp = std::reinterpret_pointer_cast<MovePlayerPacket>(packet);
        if (Mode.SelectedIndex == (int)Mode::SilentAccept)
        {
            auto player = Minecraft::ClientInstance->ThePlayer();
            if (!player) return;

            auto paip = player->getInputPacket();
            if (!paip) return;

            paip->position = mpp->position;
            paip->rotation = mpp->rotation;
            paip->headYaw = mpp->headYaw;

            // If the distance is greater than the max distance, don't send the packet
            if (UseMaxDistance.Enabled && mpp->position.Distance(*player->getPos()) > MaxDistance.Value) return;

            Minecraft::ClientInstance->packetSender->sendToServer(paip.get());
        }

        if (Mode != (int)Mode::RotateOnly)
            *cancel = true;
        else
        {
            auto player = Minecraft::ClientInstance->ThePlayer();
            if (!player) return;

            auto rot = player->GetActorRotationComponent();
            mpp->rotation = {rot->pitch, rot->yaw};
        }
    }
}