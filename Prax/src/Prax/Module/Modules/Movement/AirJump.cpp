//
// Created by vastrakai on 9/1/2023.
//

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include "AirJump.h"

void AirJump::OnEnable() {

}

void AirJump::OnDisable() {

}

float lastOnGroundPos = 0;
bool oldSpacePressed = false;
void AirJump::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto blockSource = Minecraft::ClientInstance->GetBlockSource();

    float deltaTime = Minecraft::ClientInstance->GetDeltaTime();

    if(player->isOnGround()) lastOnGroundPos = player->GetLerpedPos(deltaTime).y;

    if (!player->isOnGround()) {
        bool spacePressed = Misc::IsUsingMovementKeys(false, false, false, false, true);
        if (spacePressed && !oldSpacePressed) {
            player->setOnGround(true);
            player->jumpFromGround();
            player->setOnGround(false);
        }

        oldSpacePressed = spacePressed;
    }



}

void AirJump::OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;


    if (packet->getId() == (int) PacketID::PlayerAuthInput) {
        auto* paip = (PlayerAuthInputPacket*)packet;

        if (Misc::IsUsingMovementKeys(0, 0, 0, 0, 1) && !oldSpacePressed) {
            paip->inputFlags |= AuthInputAction::START_JUMPING | AuthInputAction::JUMPING | AuthInputAction::WANT_UP | AuthInputAction::JUMP_DOWN | AuthInputAction::NORTH_JUMP;
        }
    }
}
