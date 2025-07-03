//
// Created by vastrakai on 7/19/2024.
//

#include "NoPacket.hpp"

#include <Features/Events/PacketOutEvent.hpp>

void NoPacket::onEnable()
{
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &NoPacket::onPacketOutEvent>(this);
}

void NoPacket::onDisable()
{
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &NoPacket::onPacketOutEvent>(this);
}

void NoPacket::onPacketOutEvent(PacketOutEvent& event) const
{
    if ((mMoveOnly && (event.mPacket->getId() == PacketID::MovePlayer || event.mPacket->getId() == PacketID::PlayerAuthInput)) || !mMoveOnly)
        event.mCancelled = true;
}
