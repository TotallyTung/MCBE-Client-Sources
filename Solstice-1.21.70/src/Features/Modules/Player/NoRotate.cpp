//
// Created by ssi on 10/27/2024.
//

#include "NoRotate.hpp"
#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/Packets/MovePlayerPacket.hpp>

void NoRotate::onEnable()
{
    gFeatureManager->mDispatcher->listen<PacketInEvent, &NoRotate::onPacketInEvent>(this);
}

void NoRotate::onDisable()
{
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &NoRotate::onPacketInEvent>(this);
}

void NoRotate::onPacketInEvent(PacketInEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) {
        return;
    }

    if (event.mPacket->getId() == PacketID::MovePlayer)
    {
        auto packet = event.getPacket<MovePlayerPacket>();

        if (packet->mPlayerID == player->getRuntimeID())
        {
            auto rot = player->getActorRotationComponent();
            packet->mRot = { rot->mPitch, rot->mYaw };
        }
    }
}