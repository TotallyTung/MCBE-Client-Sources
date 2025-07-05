//
// Created by vastrakai on 10/18/2023.
//

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include "AutoZap.h"
#include "Antibot.h"

Vector3 AutoZap::LastPos = Vector3(0, 0, 0);
Vector3 AutoZap::CurrentPos = Vector3(0, 0, 0);

void AutoZap::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    auto actorList = Antibot::GetActors();

    // Exclude the player from the list
    actorList.erase(std::remove(actorList.begin(), actorList.end(), player), actorList.end());

    // Sort the list by distance to the player
    std::sort(actorList.begin(), actorList.end(), [&](Actor *a, Actor *b) {
        return player->getPos()->Distance(*a->getPos()) < player->getPos()->Distance(*b->getPos());
    });

    // Remove entities that can't be seen with player->canSee()
    actorList.erase(std::remove_if(actorList.begin(), actorList.end(), [&](Actor *a) {
        return !player->canSee(a);
    }), actorList.end());

    // return if the list is empty
    if (actorList.empty()) {
        LastPos = Vector3(0, 0, 0);
        CurrentPos = Vector3(0, 0, 0);
        return;
    }

    auto target = actorList[0];
    if (!target) {
        LastPos = Vector3(0, 0, 0);
        CurrentPos = Vector3(0, 0, 0);
        return;
    }

    Vector3 lastPos = *target->getPosPrev() - Vector3(0.f, 1.f, 0.f);
    Vector3 currentPos = *target->getPos() - Vector3(0.f, 1.f, 0.f);

    // Get the pos delta between the last tick and this tick
    Vector3 posDelta = currentPos - lastPos;
    // Add this delta to the current pos to get the next predicted pos
    Vector3 predictedPos = currentPos + (posDelta * MovementCompensation.Value);
    CurrentPos = predictedPos;

    ItemStack* item = player->supplies->GetSelectedItem();
    player->gamemode->baseUseItem(item);


}

void AutoZap::OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto paip = (PlayerAuthInputPacket *) packet;
        if (CurrentPos != Vector3(0.f, 0.f, 0.f)) {
            paip->rotation = Misc::GetRotations(player->getPos(), &CurrentPos);
            paip->headYaw = paip->rotation.y;
        }

    }
}

