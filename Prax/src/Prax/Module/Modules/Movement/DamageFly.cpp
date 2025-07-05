//
// Created by vastrakai on 12/30/2023.
//

#include "DamageFly.h"

#include <Prax/SDK/Game/Network/Other/SetEntityMotionPacket.h>
#include <Prax/Utils/System/KeyManager.h>
#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>

uintptr_t DamageFly::LastDamageTime = 0;
uintptr_t DamageFly::Tick = 0;
bool DamageFly::WasTimerBoosted = false;

void DamageFly::ResetTimer()
{
    if (TimerBoostEnabled.Enabled && WasTimerBoosted) {
        Minecraft::ClientInstance->SetTimer(20.f);
        WasTimerBoosted = false;
    }
}

void DamageFly::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    uintptr_t now = Misc::GetCurrentMs();

    static bool wasFlying = false;

    if (wasFlying)
    {
        player->GetStateVectorComponent()->velocity.x = 0;
        player->GetStateVectorComponent()->velocity.y = -0.0784000015258789;
        player->GetStateVectorComponent()->velocity.z = 0;
        wasFlying = false;
    }

    if (now > LastDamageTime) {
        Tick = 0;
        ResetTimer();
        return;
    }

    wasFlying = true;


    Tick++;

    if (TimerBoostEnabled.Enabled) {
        Minecraft::ClientInstance->SetTimer(TimerBoost.Value);
        WasTimerBoosted = true;
    }


    int maxTicks = Ticks.Value;

    if (Tick > maxTicks) {
        Tick = 0;
    }

    float speed = Speed.Value;
    float glide = Glide.Value;

    if (FrictionMode.SelectedIndex == 1) {
        if (Tick == maxTicks) {
            speed = Speed.Value;
            glide = Glide.Value;
        } // Else, multiply by the friction multiplier for the amount of ticks since 0
        else {
            for (int i = 0; i < Tick; i++) {
                speed *= SpeedFrictionMultiplier.Value;
                glide *= GlideFrictionMultiplier.Value;
            }
        }
    }

    Vector2 motion = Misc::GetMotion(player->GetActorRotationComponent()->yaw, speed);
    bool space = KeyManager::Keys[VK_SPACE];
    bool shift = KeyManager::Keys[VK_SHIFT];

    float y = glide;

    /*if (!Glide.Enabled) {
        y = Minecraft::ClientInstance->ThePlayer()->GetStateVectorComponent()->velocity.y;
        if (Tick == maxTicks) {
            y = glide;
        }
    }*/

    if (space) {
        y = speed / 10;
    } else if (shift) {
        y = -speed / 10;
    }

    player->GetStateVectorComponent()->velocity = Vector3(motion.x, y, motion.y);
}

void DamageFly::OnDisable()
{
    ResetTimer();
}

void DamageFly::OnEnable()
{

}

void DamageFly::OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled)
{
    if (!ApplyJumpFlags.Enabled) return;
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto paip = (PlayerAuthInputPacket*)packet;

        // Apply jump flags
        paip->inputFlags |= AuthInputAction::JUMPING | AuthInputAction::WANT_UP | AuthInputAction::JUMP_DOWN | AuthInputAction::NORTH_JUMP | AuthInputAction::START_JUMPING;

    }
}

void DamageFly::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (packet->getId() == (int) PacketID::SetEntityMotion) {
        auto sem = (SetEntityMotionPacket*) packet.get();
        if (sem->RuntimeID == player->getRuntimeIDComponent()->runtimeID) {
            LastDamageTime = Misc::GetCurrentMs() + (uintptr_t)FlyMs.Value;
        }
    }
}