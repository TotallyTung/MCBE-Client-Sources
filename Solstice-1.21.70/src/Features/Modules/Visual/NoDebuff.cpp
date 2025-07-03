//
// Created by alteik on 20/10/2024.
//

#include "NoDebuff.hpp"
#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/PacketID.hpp>
#include <SDK/Minecraft/Network/Packets/MobEffectPacket.hpp>
#include <Utils/GameUtils/ChatUtils.hpp>

void NoDebuff::onEnable()
{
    gFeatureManager->mDispatcher->listen<PacketInEvent, &NoDebuff::onPacketInEvent>(this);
}

void NoDebuff::onDisable()
{
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &NoDebuff::onPacketInEvent>(this);
}

void NoDebuff::onPacketInEvent(PacketInEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if(!player) return;

    if (event.mPacket->getId() == PacketID::MobEffect)
    {
        auto mep = event.getPacket<MobEffectPacket>();
        if(mep->mRuntimeId != player->getRuntimeID()) return;

        if(mep->mEffectId == EffectType::Blindness) event.cancel();
        if(mep->mEffectId == EffectType::Nausea) event.cancel();
        if(mep->mEffectId == EffectType::Wither) event.cancel();
        if(mep->mEffectId == EffectType::Darkness)event.cancel();
    }
}