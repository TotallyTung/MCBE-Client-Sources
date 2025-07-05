//
// Created by vastrakai on 9/17/2023.
//

#include <Prax/Utils/String.h>
#include <Prax/SDK/Game/Network/Other/ActorEventPacket.h>
#include <Prax/SDK/Game/Network/Other/CompletedUsingItemPacket.h>
#include <Prax/Module/Modules/Visual/PlaceHighlights.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/SDK/Game/Render/LevelRenderer.h>
#include "ParticleSpam.h"

#include <Prax/Module/Modules/Combat/Antibot.h>

#include "Prax/SDK/Game/Network/Other/AnimatePacket.h"

uint64_t ParticleSpam::LastBurst = 0;
uint64_t ParticleSpam::LastDelay = 0;
bool ParticleSpam::Bursting = false;

void ParticleSpam::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();

    uint64_t now = Misc::GetCurrentMs();

    if (Delay.Enabled)
    {
        if (now - LastDelay < DelayTime.Value) return;
    }

    for (int i = 0; i < (int)Intensity.Value; i++)
    {
        if (!Bursting)
        {
            Bursting = true;
            LastBurst = now;
        }
        std::shared_ptr<AnimatePacket> animatePacket = MinecraftPackets::CreatePacket<AnimatePacket>();
        animatePacket->ActorRuntimeID = player->GetRuntimeID();
        animatePacket->ActionID = 3;
        animatePacket->Data = FLT_MAX;
        Minecraft::ClientInstance->packetSender->sendToServer(animatePacket.get());
    }

    // Only set last delay if the last burst was more than the burst time ago
    if (now - LastBurst > BurstTime.Value)
    {
        LastDelay = now;
        Bursting = false;
    }
}