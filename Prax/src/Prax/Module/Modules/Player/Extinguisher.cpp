//
// Created by vastrakai on 12/21/2023.
//

#include "Extinguisher.h"

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/Utils/Math.h>

#include "Regen.h"

bool Extinguisher::Rotate = false;
Vector3 Extinguisher::CurrentPosition = Vector3(0, 0, 0);

void Extinguisher::OnTick()
{
    if (Regen::IsBreaking) return;
    auto player = Minecraft::ClientInstance->ThePlayer();
    std::vector<BlockPosPair> blocks = Misc::GetBlocksWithinDistance2(player->getPos(), Range.Value);
    bool outBool = false;

    Vector3 blockPos = {};

    for (auto& block : blocks) {

        if (block.block->blockLegacy->id == 51) { // 51 == minecraft:fire
            blockPos = block.blockPos;
            break;
        }
    }

    if (blockPos == Vector3(0, 0, 0)) return;

    player->gamemode->startDestroyBlock(blockPos, -6, outBool);
    if (outBool)
    {
        Rotate = true;
        CurrentPosition = blockPos;
    }
}

void Extinguisher::OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto paip = (PlayerAuthInputPacket*)packet;
        if (Rotate) {
            Vector2 rotations = Misc::GetRotations(player->getPos(), &CurrentPosition);
            paip->rotation = rotations;
            paip->headYaw = rotations.y;
            Rotate = false;
        }
    }
}
