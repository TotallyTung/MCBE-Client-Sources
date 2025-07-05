//
// Created by vastrakai on 10/28/2023.
//

#include "AntiImmobile.h"
#include <Prax/Utils/Misc.h>
#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/UI/Notifications.h>

#include "Prax/Hook/Hooks/RakPeerHooks.h"

uint64_t AntiImmobile::LastDimensionChange = 0;
uint64_t AntiImmobile::LastTeleport = 0;

#define NOW Misc::GetCurrentMs()

bool isAirBlock(Vector3 pos) {
    auto block = Minecraft::ClientInstance->GetBlockSource()->getBlock(pos);
    return block->blockLegacy->getid() == 0 || block->blockLegacy->getid() == 95;
}

void AntiImmobile::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (Mode == ClientFalse)
        player->setStatusFlag(ActorFlags::Noai, false);
    else if (Mode == Clip || Mode == HiveSky) {
        if (LastDimensionChange == 0 || LastTeleport == 0) {
            LastDimensionChange = 0;
            LastTeleport = 0;
        }
        if (NOW - LastDimensionChange > 1000 ||
            NOW - LastTeleport > 1000 ||
            !player->getStatusFlag(ActorFlags::Noai) ||
            !Minecraft::ClientInstance->mcGame->canUseKeys()) return;

        Vector3 playerPos = *player->getPos() - Vector3(0.f, player->GetHeightOffset(), 0.f);

        bool isAir = true;
        Vector3 currentBlockPos = playerPos;
        // floor the block position
        currentBlockPos.x = floorf(currentBlockPos.x);
        currentBlockPos.z = floorf(currentBlockPos.z);

        while (isAir && currentBlockPos.y > 0.f) {
            currentBlockPos.y -= 1.f;
            isAir = isAirBlock(currentBlockPos);
        }

        if (!isAir)
        {
            // center the player on the block
            currentBlockPos.x += 0.5f;
            currentBlockPos.z += 0.5f;
            player->setPosition(currentBlockPos + Vector3(0.f, player->GetHeightOffset() + 1, 0.f));
            player->GetStateVectorComponent()->pos = currentBlockPos + Vector3(0.f, player->GetHeightOffset() + 1, 0.f);
            player->GetStateVectorComponent()->posOld = currentBlockPos + Vector3(0.f, player->GetHeightOffset() + 1, 0.f);

        }

        //LastDimensionChange = 0;
        //LastTeleport = 0;

        if (Mode == HiveSky) {
            if (NOW - LastTeleport > 5000) {
                if (!RakPeerHooks::Ticking) {
                    RakPeerHooks::TicksToSkip = 100;
                    RakPeerHooks::Ticking = true;
                } else {
                    Notifications::Notify(Prax::ClientName, "Conflicting NetSkipping! - Report this to kayla!", 2.5f);
                }

                player->setStatusFlag(ActorFlags::Noai, false);

            } else if (NOW - LastTeleport < 9900) {
                player->setStatusFlag(ActorFlags::Noai, true);

            }
        }
    }
}

void AntiImmobile::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel)
{
    if (Mode == Clip || Mode == HiveSky)
    {
        if (packet->getId() == (int)PacketID::ChangeDimension)
        {
            LastDimensionChange = NOW;
        }
        else if (packet->getId() == (int)PacketID::MovePlayer)
        {
            LastTeleport = NOW;
        }
    }
}
