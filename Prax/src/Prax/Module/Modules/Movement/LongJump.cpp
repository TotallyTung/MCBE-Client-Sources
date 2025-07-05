//
// Created by vastrakai on 8/27/2023.
//

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/Module/ModuleManager.h>
#include "LongJump.h"

bool LongJump::hasJumped = false;

void LongJump::OnDisable() {
    Minecraft::ClientInstance->SetTimer(20.f);
    hasJumped = false;

    if (DisableModules.Enabled) {
        for (auto mod : ModuleManager::Modules) {
            if (mod == this) continue;
            if (mod->Enabled && mod->KeyId != 0 && mod->KeyId == this->KeyId) {
                mod->Toggle();
            }
        }
    }
}

void LongJump::OnTick(){
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    float speedVal = this->Speed.Value;
    float heightVal = this->Height.Value;
    float timerVal = this->Timer.Value;

    Vector2 motion = Misc::GetMotion(player->GetActorRotationComponent()->yaw, speedVal);
    Vector3 velocity = Vector3(motion.x, heightVal, motion.y);

    if (timerVal != -0.1f) Minecraft::ClientInstance->SetTimer(timerVal);

    if (player->isOnGround() && !hasJumped){
        player->GetStateVectorComponent()->velocity = velocity;
        hasJumped = true;
    }

    if (player->isOnGround() && !player->wasOnGround()) Toggle(); // Disable when we land


}

void LongJump::OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        auto paip = (PlayerAuthInputPacket *) packet;

        if (Misc::IsUsingMovementKeys()) paip->inputFlags |= AuthInputAction::JUMPING | AuthInputAction::WANT_UP | AuthInputAction::JUMP_DOWN | AuthInputAction::NORTH_JUMP;
        if (!player->isOnGround() && player->wasOnGround() && Misc::IsUsingMovementKeys()) {
            paip->inputFlags |= AuthInputAction::START_JUMPING;
        }
    }
}