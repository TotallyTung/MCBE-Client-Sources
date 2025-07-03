//
// Created by ssi on 10/26/2024.
//

#include "DamageBoost.hpp"

#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/Packets/Packet.hpp>
#include <SDK/Minecraft/Network/Packets/SetActorMotionPacket.hpp>

void DamageBoost::onEnable()
{
    gFeatureManager->mDispatcher->listen<PacketInEvent, &DamageBoost::onPacketInEvent>(this);
}

void DamageBoost::onDisable()
{
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &DamageBoost::onPacketInEvent>(this);
}

void DamageBoost::onPacketInEvent(PacketInEvent& event)
{
    if (event.mPacket->getId() == PacketID::SetActorMotion) {
        auto sam = event.getPacket<SetActorMotionPacket>();
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player || sam->mRuntimeID != player->getRuntimeID()) return;

        float boostSpeed = mOnGroundCheck.mValue ?
                           (player->isOnGround() ? mGroundSpeed.mValue : mOffGroundSpeed.mValue) :
                           mSpeed.mValue;

        glm::vec2 motion = MathUtils::getMotion(player->getActorRotationComponent()->mYaw, (boostSpeed) / 10);

        auto stateVector = player->getStateVectorComponent();
        stateVector->mVelocity = {motion.x, stateVector->mVelocity.y, motion.y};

        event.cancel();
    }
}