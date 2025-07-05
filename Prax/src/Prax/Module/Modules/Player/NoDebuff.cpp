//
// Created by vastrakai on 5/9/2024.
//

#include "NoDebuff.h"

void NoDebuff::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel)
{
    if (packet->getId() != (int)PacketID::MobEffect) return;
    auto player = Minecraft::ClientInstance->ThePlayer();

    auto mobEffect = std::reinterpret_pointer_cast<MobEffectPacket>(packet);

    if (mobEffect->mRuntimeId != player->getRuntimeIDComponent()->runtimeID) return;

    if (NoSlowness.Enabled && mobEffect->mEffectId == EffectType::Slowness) *cancel = true;
    if (NoMiningFatigue.Enabled && mobEffect->mEffectId == EffectType::MiningFatigue) *cancel = true;
    if (NoBlindness.Enabled && mobEffect->mEffectId == EffectType::Blindness) *cancel = true;
    if (NoNausea.Enabled && mobEffect->mEffectId == EffectType::Nausea) *cancel = true;
    if (NoWeakness.Enabled && mobEffect->mEffectId == EffectType::Weakness) *cancel = true;

}
