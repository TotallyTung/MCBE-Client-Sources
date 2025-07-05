//
// Created by vastrakai on 4/28/2024.
//

#include "TestFly.h"

#include <Prax/SDK/Game/Network/Other/SetEntityMotionPacket.h>
//
// Created by vastrakai on 2/23/2024.
//

float TestFly::EnableY = 0;
uint64_t TestFly::LastJump = 0;
uint64_t TestFly::LastDamage = 0;

#define PLAYER_HEIGHT 1.62f;\

float TestFly::CurrentFriction = 1.f;

void TestFly::AddDebugMessage(const std::string& message) {
    if (Debug.Enabled) {
        Logger::Write("TestFly", message, Logger::Debug);
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (player) {
            player->addChatMessage("&6[TestFly &7Debug&6] &r" + message, 1);
        }
    }
}

void TestFly::Jump()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    bool onGround = player->isOnGround();
    player->setOnGround(true);
    player->jumpFromGround();
    player->setOnGround(onGround);

    Vector2 motion = Misc::GetMotion(player->GetActorRotationComponent()->yaw, SpeedSetting.Value);
    player->GetStateVectorComponent()->velocity = Vector3(motion.x, player->GetStateVectorComponent()->velocity.y, motion.y);

    AddDebugMessage("Jumping");
    CurrentFriction = 1.f;
};

void TestFly::OnEnable() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    EnableY = player->getPos()->y;
    LastJump = Misc::GetCurrentMs();
    AddDebugMessage("Enabled");





}

void TestFly::OnDisable() {
    EnableY = 0;
    Minecraft::ClientInstance->SetTimer(20.f);
    AddDebugMessage("Disabled");
    if (ResetOnDisable.Enabled) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        // only set velocity if the player should be flying
        if (OnDamageOnly.Enabled && Misc::GetCurrentMs() > LastDamage) return;
        player->GetStateVectorComponent()->velocity = Vector3(0.f, player->GetStateVectorComponent()->velocity.y, 0.f);
        LastDamage = 0;
    }
}



void TestFly::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    static int flyTicks = 0;

    static bool wasFlying = false;

    if (OnDamageOnly.Enabled && Misc::GetCurrentMs() > LastDamage) {
        if (wasFlying)
        {
            EnableY = player->getPos()->y;
            if (ResetOnDisable.Enabled)
                player->GetStateVectorComponent()->velocity = Vector3(0.f, player->GetStateVectorComponent()->velocity.y, 0.f);

            wasFlying = false;
        }

        Minecraft::ClientInstance->SetTimer(20.f);
        CurrentFriction = 1.f;

        flyTicks = 0;
        return;
    }

    wasFlying = true;

    if (PauseDuringTimeframe.Enabled) {
        if (flyTicks < static_cast<int>(PauseStart.Value)) {
            Minecraft::ClientInstance->SetTimer(TimerBoost.Value);
            Logger::Write("TestFly", "currentTick: " + std::to_string(flyTicks), Logger::Debug);
        } else if (flyTicks >= static_cast<int>(PauseStart.Value) && flyTicks <= static_cast<int>(PauseEnd.Value)) {
            Minecraft::ClientInstance->SetTimer(PausedTimer.Value);
            AddDebugMessage("Paused, currentTick: " + std::to_string(flyTicks));
            player->GetStateVectorComponent()->velocity = Vector3(0.f, player->GetStateVectorComponent()->velocity.y, 0.f);
            flyTicks++;
            return;
        } else if (flyTicks > static_cast<int>(PauseEnd.Value)) {
            AddDebugMessage("Resetting flyTicks");
            Minecraft::ClientInstance->SetTimer(TimerBoost.Value);
            flyTicks = 0;
        }
    } else {
        Minecraft::ClientInstance->SetTimer(TimerBoost.Value);
    }


    if (player->isOnGround())
    {
        AddDebugMessage("On Ground");
        CurrentFriction = 1.f;
    }
    else if (SpeedFriction.Enabled) {
        CurrentFriction *= Friction.Value;

        float speed = SpeedSetting.Value * CurrentFriction;

        Vector2 motion = Misc::GetMotion(player->GetActorRotationComponent()->yaw, SpeedSetting.Value * CurrentFriction);
        player->GetStateVectorComponent()->velocity = Vector3(motion.x, player->GetStateVectorComponent()->velocity.y, motion.y);
    }

    Minecraft::ClientInstance->SetTimer(TimerBoost.Value);

    if (player->getPos()->y < EnableY && !player->isOnGround()) {
        if (Misc::GetCurrentMs() - LastJump > static_cast<uint64_t>(JumpDelay.Value) * static_cast<uint64_t>(1000)) {
            Jump();
            LastJump = Misc::GetCurrentMs();
        }

        AddDebugMessage("Height: " + std::to_string(player->getPos()->y - EnableY));
        EnableY -= HeightLoss.Value;
    }

    if (player->isOnGround() && ResetOnGround.Enabled) {
        EnableY = player->getPos()->y;
        AddDebugMessage("Resetting height [OnGround]");
    }

    flyTicks++;
}

void TestFly::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (packet->getId() == (int) PacketID::SetEntityMotion) {
        auto sem = reinterpret_cast<SetEntityMotionPacket*>(packet.get());
        if (sem->RuntimeID == player->getRuntimeIDComponent()->runtimeID) {
            LastDamage = Misc::GetCurrentMs() + static_cast<uintptr_t>(FlyTime.Value) * 1000;
            EnableY = player->getPos()->y;
            AddDebugMessage("Damage taken");
        }
    }
}

