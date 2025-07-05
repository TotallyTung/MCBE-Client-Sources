//
// Created by vastrakai on 8/23/2023.
//

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include "AutoSprint.h"

bool wasUsingMovementKeys = false;

void AutoSprint::OnTick(){
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto input = player->GetMoveInputComponent();

    input->isSprinting = true;
}

void AutoSprint::OnPacket(class LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    auto moveInput = player->GetMoveInputComponent();

    bool usingMovementKeys = moveInput->backward || moveInput->forward || moveInput->left || moveInput->right;
    if (!OmniSprint.Enabled) usingMovementKeys = moveInput->forward;



    if (usingMovementKeys || Always.Enabled) {
        player->GetMoveInputComponent()->isSprinting = true;
    } else {
        player->GetMoveInputComponent()->isSprinting = false;
    }

    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto paip = (PlayerAuthInputPacket *) packet;


        if (Always.Enabled) {
            paip->inputFlags |= AuthInputAction::SPRINTING | AuthInputAction::SPRINT_DOWN;
            paip->inputFlags |= AuthInputAction::START_SPRINTING;
            paip->inputFlags &= ~AuthInputAction::STOP_SPRINTING;
        } else
        if (usingMovementKeys) {
            paip->inputFlags |= AuthInputAction::SPRINTING | AuthInputAction::SPRINT_DOWN;
            paip->inputFlags |= AuthInputAction::START_SPRINTING;
            paip->inputFlags &= ~AuthInputAction::STOP_SPRINTING;
            wasUsingMovementKeys = true;
        } else if (wasUsingMovementKeys) {
            paip->inputFlags &= ~AuthInputAction::SPRINTING | AuthInputAction::SPRINT_DOWN;
            paip->inputFlags |= AuthInputAction::STOP_SPRINTING;
            wasUsingMovementKeys = false;
        }

    }
}