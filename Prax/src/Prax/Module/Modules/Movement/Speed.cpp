#pragma once

#include <Prax/Utils/Render/D2D.h>
#include <Prax/Utils/Math.h>
#include <Prax/SDK/Game/Network/Network.h>
#include <imgui_internal.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/Utils/System/KeyManager.h>
#include "Speed.h"
#include "../../../UI/ClickGui.h"
#include <Prax/Module/Modules/Player/Regen.h>
#include <Prax/Module/Modules/World/Timer.h>
#include <Prax/Module/Modules/World/TimerSlider.h>
#include <Prax/SDK/Game/Network/Other/SetEntityMotionPacket.h>
#include <Prax/Module/Modules/Misc/NetSkip.h>
#include <Prax/Module/Modules/Visual/BPSDisplay.h>
#include <Prax/Module/Modules/Movement/TargetStrafe.h>
#include <Prax/Module/Modules/World/Scaffold.h>

#include "DamageBoost.h"
#include "InvMove.h"
#include "TestFly.h"
#include "Prax/Hook/Hooks/RakPeerHooks.h"
#include "Prax/SDK/Game/Network/Other/MobEffectPacket.h"

float speed = 0.f;
bool wasDesyncing = false;

static float friction = 1.f;

std::map<EffectType, uint64_t> effectTimers = {};

static float staticDamageBoostInc = 0.f;
static uint64_t staticDamageBoostStartTime = 0;
static uint64_t damageTimerEndTime = 0.f;

void Speed::OnEnable() {
    friction = 1.f;

    staticDamageBoostInc = 0;
    damageTimerEndTime = 0;
}

void Speed::OnDisable() {
    InvMove::JumpOverride = false;
    Minecraft::ClientInstance->SetTimer(20.f);
    Minecraft::ClientInstance->SetTimerMultiplier(1.f);
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (NoStep.Enabled) player->GetMaxAutoStepComponent()->maxAutoStep = 0.562500f;

    if (ResetMotion.Enabled) {
        player->GetStateVectorComponent()->velocity.y = -0.08f;
    }

    if (wasDesyncing) {
        RakPeerHooks::TicksToSkip = 0;
        RakPeerHooks::MsToSkip = 0;
        RakPeerHooks::Ticking = false;
        RakPeerHooks::RenderCurrentAndLastPos = false;
        wasDesyncing = false;
    }
}

int Speed::OffGroundTicks = 0;

float moveYaw = 0.f;
float oldMoveYaw = 0.f;
uint64_t lastKbTime = 0;

// gpt4 <3

double normalizeAngle(double angle) {
    angle = fmod(angle, 360.0);
    if (angle < 0)
        angle += 360.0;
    return angle;
}

double calculateShortestAngleDifference(double playerYaw, double movementYaw) {
    // Normalize angles to be within [0, 360) degrees
    playerYaw = normalizeAngle(playerYaw);
    movementYaw = normalizeAngle(movementYaw);

    // Calculate the difference and adjust for the circular nature of angles
    double diff = movementYaw - playerYaw;
    diff = normalizeAngle(diff); // Ensure diff is in [0, 360)

    // If the difference is greater than 180 degrees, the shortest path is in the opposite direction
    if (diff > 180) {
        diff = 360 - diff;
    }

    return diff;
}

int getSpellBook()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return -1;

    ItemStack* itemStack = nullptr;
    int itemSlot = -1;

    for (int i = 0; i < 36; i++) {
        auto item = player->supplies->GetItemAtSlot(i);

        if (item && item->item) {
            if (String::ContainsIgnoreCase(item->GetCustomName(), "Spell of Swiftness")) {
                itemStack = item;
                itemSlot = i;
                break;
            }
        }
    }

    if (itemStack == nullptr) {
        return -1;
    }

    return itemSlot;
}

bool Speed::IsApplyingDamageTimer() {
    return DamageTimer.Enabled && damageTimerEndTime > Misc::GetCurrentMs();
}

void Speed::OnTick() {
    static auto targetStrafe = ModuleManager::GetModule<TargetStrafe>();
    static auto scaffold = ModuleManager::GetModule<Scaffold>();
    static auto timerModule = ModuleManager::GetModule<Timer>();
    static auto timerSliderModule = ModuleManager::GetModule<TimerSlider>();
    static auto invMove = ModuleManager::GetModule<InvMove>();
    static auto netskip = ModuleManager::GetModule<NetSkip>();
    static auto testFly = ModuleManager::GetModule<TestFly>();

    if (testFly->Enabled && !testFly->OnDamageOnly.Enabled ||
        testFly->OnDamageOnly.Enabled && Misc::GetCurrentMs() < testFly->LastDamage && testFly->Enabled) {
        return;
    }

    if (HiveMode.Enabled) {
        if (netskip->DisableWhenScaffolding.Enabled && scaffold && scaffold->Enabled) {
            if (RakPeerHooks::Ticking) {
                RakPeerHooks::TicksToSkip = 0;
                RakPeerHooks::MsToSkip = 0;
                RakPeerHooks::Ticking = false;
                RakPeerHooks::RenderCurrentAndLastPos = false;
                wasDesyncing = false;
            }
        } else if (!IsApplyingDamageTimer()) {
            RakPeerHooks::MsToSkip = (int) 101;
            RakPeerHooks::TicksToSkip = 0;
            RakPeerHooks::Ticking = true;
            wasDesyncing = true;
        } else if (IsApplyingDamageTimer() && DamageTimerDesync.Enabled) {
            Logger::Write("Speed", "DamageTimerDesync: setting tickstoskip to " + std::to_string(DamageTimerDesyncTicks.Value), Logger::LogType::Debug);
            RakPeerHooks::MsToSkip = (int)0;
            RakPeerHooks::TicksToSkip = DamageTimerDesyncTicks.Value;
            RakPeerHooks::Ticking = true;
            wasDesyncing = true;
        }
    }

    if (DisableWhileScaffolding.Enabled && scaffold && scaffold->Enabled) {
        return;
    }

    if (targetStrafe->Enabled) {
        static_cast<TargetStrafe*>(targetStrafe)->strafe();
    }


    if (TimerBoost.Value > 0 && !timerModule->Enabled && !timerSliderModule->Enabled) {
        Minecraft::ClientInstance->SetTimer(TimerBoost.Value);
    }



    const std::string& name = Minecraft::ClientInstance->GetScreenName();

    if (name != "hud_screen" && !invMove->Enabled || name == "chat_screen") return;

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    static bool wasUsingMovementKeys = false;

    bool isUsingMovementKeys = Misc::IsUsingMovementKeys();

    bool instantDirection = InstantDirection.Enabled;
    bool strafeSetting = StrafeSetting.Enabled;

    if (NoStrafeWhileScaffolding.Enabled) {
        if (scaffold && scaffold->Enabled) {
            strafeSetting = false;
            instantDirection = false;
        }
    }
    auto input = player->GetMoveInputComponent();

    bool isUsingStrafeKeys = input->left || input->right;
        //KeyManager::Keys['A'] || KeyManager::Keys['D'];


    if (!Misc::IsUsingMovementKeys()) {
        if(wasDesyncing) {
            auto player = Minecraft::ClientInstance->ThePlayer();
            RakPeerHooks::TicksToSkip = 0;
            RakPeerHooks::MsToSkip = 0;
            RakPeerHooks::Ticking = false;
            RakPeerHooks::RenderCurrentAndLastPos = false;
            wasDesyncing = false;
        }

        if (wasUsingMovementKeys && !isUsingMovementKeys && ResetMotion.Enabled) {
            player->GetStateVectorComponent()->velocity.y = -0.08f; // reset y motion
            wasUsingMovementKeys = isUsingMovementKeys;
        }

        return;
    }

    wasUsingMovementKeys = isUsingMovementKeys;



    bool isOnGround = player->isOnGround();

    float speedSetting = SpeedSetting.Value;

    if (UseStrafeSpeed.Enabled && isUsingStrafeKeys) {
        speedSetting = StrafeSpeed.Value;
    }

    if (RandomizeSpeed.Enabled) {
        float minspeed = RandomMinSetting.Value;
        float maxspeed = RandomMaxSetting.Value;
        speedSetting *= Math::RandomFloat(minspeed, maxspeed);
    }

    if (isOnGround) {
        OffGroundTicks = 0;
    } else {
        OffGroundTicks++;
    }
    uint64_t endTime = staticDamageBoostStartTime + (uint64_t)StaticDamageBoostMs.Value;

    bool timePassed = false;
    if (StaticDamageBoostMs.Value > 0) {
        if (Misc::GetCurrentMs() > endTime) {
            timePassed = true;
        }
    }

    if (StaticDamageBoost.Enabled && staticDamageBoostInc > 0 && !timePassed) {
        if (StaticDamageBoostMinInc.Value > 0) staticDamageBoostInc -= fmax(StaticDamageBoostMinInc.Value, StaticDamageBoostFalloff.Value * staticDamageBoostInc);
        speedSetting *= 1 + staticDamageBoostInc;

        if (DamageStrafe.Enabled && staticDamageBoostInc > BoostNeededToStrafe.Value) {
            instantDirection = true;
            strafeSetting = true;
        }
    }

    static bool wasUsingDamageTimer = false;

    if (DamageTimer.Enabled && damageTimerEndTime > Misc::GetCurrentMs()) {
        auto amt = DamageTimerBoost.Value;

        if (DamageTimerCombine.Enabled) {
            amt /= (20 / TimerBoost.Value);
        }

        Logger::Write("Speed", "DmgTimer: setting timer to " + std::to_string(amt) + ", revert in " + std::to_string(damageTimerEndTime - Misc::GetCurrentMs()), Logger::LogType::Debug);
        Minecraft::ClientInstance->SetTimer(amt);
        wasUsingDamageTimer = true;
    } else if (wasUsingDamageTimer) {
        Minecraft::ClientInstance->SetTimer(20.f);
        wasUsingDamageTimer = false;
    }

    static bool lastSpace = false;
    bool space = KeyManager::Keys[VK_SPACE];
    static int64_t lastSpellbookUse = 0;
    bool spellbookTimeExpired = Misc::GetCurrentMs() - lastSpellbookUse > 4700;

    if (UseSwiftnessBook.Enabled) {
        bool hasSpeed = false;
        // Check da effectTimers
        for (auto& [effect, time] : effectTimers) {
            if (time > Misc::GetCurrentMs()) {
                if (effect == EffectType::Speed) {
                    // If there is less than 100ms+RakPeerHooks::LastPing time left on the speed effect, set hasSpeed to false
                    if (time - Misc::GetCurrentMs() < 100 + RakPeerHooks::LastPing) {
                        hasSpeed = false;
                    } else {
                        hasSpeed = true;
                    }
                }
            }
        }

        int spellBook = getSpellBook();
        if (spellBook != -1 && space && !lastSpace && !hasSpeed)
        {
            Misc::SwitchAndEquip(spellBook);
            lastSpellbookUse = Misc::GetCurrentMs();
            Logger::Write("Speed", "Using spellbook", Logger::LogType::Debug);
        }

        if (hasSpeed)
        {
            // set step
            player->GetMaxAutoStepComponent()->maxAutoStep = 0.562500f;
        }

        if (space && hasSpeed || !HoldSpace.Enabled && hasSpeed)
        {
            // set space in moveinput to false
            InvMove::JumpOverride = true;
            if (SwiftnessJump.Enabled)
            {
                if (LowHopSetting.Enabled) {
                    // If LowHopHeight setting is within 0.01 of 0.42, use player->jumpFromGround()
                    if (LowHopBoost.Enabled) {
                        player->jumpFromGround();
                    }

                    if (player->isOnGround()) {
                        player->GetStateVectorComponent()->velocity.y = LowHopHeight.Value;
                    }

                }
            }

            input->isJumping = false;


            Vector3 velocity = player->GetStateVectorComponent()->velocity;
            float movementSpeed = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

            float movementYaw = atan2(velocity.z, velocity.x);
            float movementYawDegrees = movementYaw * (180.0f / IM_PI) - 90.f;

            float playerYawDegrees = player->GetActorRotationComponent()->yaw +
                                     Misc::GetRotationKeyOffset(true);

            float yawDifference = playerYawDegrees - movementYawDegrees;
            float yawDifferenceRadians = yawDifference * (IM_PI / 180.0f);
            float newMovementYaw = movementYaw + yawDifferenceRadians;

            uint64_t now = Misc::GetCurrentMs();
            Minecraft::ClientInstance->SetTimerMultiplier(SwiftnessTimer.Value / 20.0f);

            float friction = 1.f;
            // for each offgroundtick mul friction by 0.99
            for (int i = 0; i < OffGroundTicks; i++) {
                friction *= SwiftnessFriction.Value;
            }

            if (Misc::IsUsingMovementKeys())
            {
                bool speedOffGround = SwiftnessOffGround.Enabled;
                if (speedOffGround && player->isOnGround()) {
                    movementSpeed = SwiftnessSpeed.Value * friction;
                } else if (!speedOffGround) {
                    movementSpeed = SwiftnessSpeed.Value * friction;
                }
            }
            Vector3 newVelocity = Vector3(cosf(newMovementYaw) * movementSpeed, velocity.y,
                                      sinf(newMovementYaw) * movementSpeed);

            player->GetStateVectorComponent()->velocity = newVelocity;
            return;
        }

        InvMove::JumpOverride = false;
    }
    Minecraft::ClientInstance->SetTimerMultiplier(1.f);

    if (NoStep.Enabled) player->GetMaxAutoStepComponent()->maxAutoStep = 0.01f;

    lastSpace = space;


    static bool wasFrictionReset = false;

    if (!ResetOnce.Enabled) wasFrictionReset = false;
    else if (ResetOnce.Enabled && isOnGround) {
        wasFrictionReset = false;
    }

    if (ResetFriction.Enabled && OffGroundTicks >= ResetFrictionTick.Value && !wasFrictionReset) {
        friction = FrictionToReset.Value;
        OffGroundTicks = 0;
    }

    if (DownMotionMode.SelectedIndex == PredictVel) {
        auto stateVector = player->GetStateVectorComponent();

        if (OffGroundTicks == DownMotionTick.Value) {
            for (int i = 0; i < DownMotionPredict.Value; i++) {
                stateVector->velocity.y = player->PredictNextMotion(stateVector->velocity.y);
            }
        }
    } else if (DownMotionMode.SelectedIndex == SetVel) {
        if (OffGroundTicks == DownMotionTick.Value) {
            player->GetStateVectorComponent()->velocity.y = -DownMotionPredict.Value;
        }
    }

    if (SecondDownMotion.Enabled) {
        auto stateVector = player->GetStateVectorComponent();

        if (OffGroundTicks == SecondDownMotionTick.Value) {
            for (int i = 0; i < SecondDownMotionPredict.Value; i++) {
                stateVector->velocity.y = player->PredictNextMotion(stateVector->velocity.y);
            }
        }
    }

    static auto nsm = ModuleManager::GetModule<NetSkip>();

    switch (SpeedModeSetting.SelectedIndex)
    {
    case Flareon: {
            Vector3 velocity = player->GetStateVectorComponent()->velocity;
            float movementSpeed = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
            if (Misc::IsUsingMovementKeys() && player->isCollidingHorizontal() && !player->isOnGround()) {
                //movementSpeed *= 0.2f;
            }
            if (player->isOnGround() && Misc::IsUsingMovementKeys()) {
                movementSpeed = speedSetting / 10;
                if (space && Desync.Enabled){
                    movementSpeed = DesyncSpeed.Value / 10;
                }
            } else if (!player->isOnGround()) {
                movementSpeed *= FrictionSetting.Value;
            }
            float movementYaw = atan2(velocity.z, velocity.x);
            float movementYawDegrees = movementYaw * (180.0f / IM_PI) - 90.f;

            float playerYawDegrees = player->GetActorRotationComponent()->yaw +
                                     Misc::GetRotationKeyOffset(strafeSetting);

            float yawDifference = playerYawDegrees - movementYawDegrees;
            float yawDifferenceRadians = yawDifference * (IM_PI / 180.0f);
            float newMovementYaw = movementYaw + yawDifferenceRadians;
            if (!instantDirection && player->isOnGround()) newMovementYaw = movementYaw;

            Vector3 newVelocity = Vector3(cosf(newMovementYaw) * movementSpeed, velocity.y,
                                          sinf(newMovementYaw) * movementSpeed);

            if (space && Desync.Enabled) {
                input->isJumping = false;
                Minecraft::ClientInstance->SetTimer(DesyncTimer.Value);
                RakPeerHooks::Ticking = true;
                RakPeerHooks::MsToSkip = DesyncMilliseconds.Value;
                RakPeerHooks::Visualize = nsm->VisualizeSetting.Enabled;
                RakPeerHooks::RenderCurrentAndLastPos = nsm->RenderCurrentAndLastPosSetting.Enabled;
                wasDesyncing = true;
            } else if (Desync.Enabled) {
                RakPeerHooks::Ticking = false;
                RakPeerHooks::MsToSkip = 0;
                wasDesyncing = false;
            }

            player->GetStateVectorComponent()->velocity = newVelocity;
            break;
    }
    case Friction: {
            Vector3 velocity = player->GetStateVectorComponent()->velocity;
            float movementSpeed = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);


            if (!player->isOnGround() && Misc::IsUsingMovementKeys() && player->wasOnGround()) {
                if (!space || !Desync.Enabled)
                    movementSpeed += speedSetting / 10 / 10;
                else if (space && Desync.Enabled)
                    movementSpeed += DesyncSpeed.Value / 10 / 10;
            }
            else if (Misc::IsUsingMovementKeys() && !player->isOnGround() && !player->wasOnGround() && player->GetFallDistance() > 0) {
                movementSpeed *= FrictionSetting.Value;
            }


            float movementYaw = atan2(velocity.z, velocity.x);
            float movementYawDegrees = movementYaw * (180.0f / IM_PI) - 90.f;

            float playerYawDegrees = player->GetActorRotationComponent()->yaw +
                                     Misc::GetRotationKeyOffset(StrafeSetting.Enabled);

            float yawDifference = playerYawDegrees - movementYawDegrees;
            float yawDifferenceRadians = yawDifference * (IM_PI / 180.0f);
            float newMovementYaw = movementYaw + yawDifferenceRadians;
            if (!instantDirection) newMovementYaw = movementYaw;

            Vector3 newVelocity = Vector3(cosf(newMovementYaw) * movementSpeed, velocity.y,
                                          sinf(newMovementYaw) * movementSpeed);

            if (space && Desync.Enabled) {
                input->isJumping = false;
                Minecraft::ClientInstance->SetTimer(DesyncTimer.Value);
                RakPeerHooks::Ticking = true;
                RakPeerHooks::MsToSkip = DesyncMilliseconds.Value;
                RakPeerHooks::Visualize = nsm->VisualizeSetting.Enabled;
                RakPeerHooks::RenderCurrentAndLastPos = nsm->RenderCurrentAndLastPosSetting.Enabled;
                wasDesyncing = true;
            } else if (Desync.Enabled) {
                RakPeerHooks::Ticking = false;
                RakPeerHooks::MsToSkip = 0;
                wasDesyncing = false;
            }

            player->GetStateVectorComponent()->velocity = newVelocity;
            break;
    }
#ifndef __RELEASEFREE__
    case UwU: {
        Vector3 velocity = player->GetStateVectorComponent()->velocity;
        float movementSpeed = fmax(sqrt(velocity.x * velocity.x + velocity.z * velocity.z), SpeedSetting.Value / 10);


        if (!player->isOnGround() && Misc::IsUsingMovementKeys() && player->wasOnGround()) {
            //movementSpeed += speedSetting / 10 / 10;
        }
        else if (Misc::IsUsingMovementKeys() && !player->isOnGround() && !player->wasOnGround() && player->GetFallDistance() > 0) {
            movementSpeed *= FrictionSetting.Value;
        }


        float movementYaw = atan2(velocity.z, velocity.x);
        float movementYawDegrees = movementYaw * (180.0f / IM_PI) - 90.f;

        float playerYawDegrees = player->GetActorRotationComponent()->yaw +
                                 Misc::GetRotationKeyOffset(StrafeSetting.Enabled);

        float yawDifference = playerYawDegrees - movementYawDegrees;
        float yawDifferenceRadians = yawDifference * (IM_PI / 180.0f);
        float newMovementYaw = movementYaw + yawDifferenceRadians;
        if (!instantDirection) newMovementYaw = movementYaw;

        Vector3 newVelocity = Vector3(cosf(newMovementYaw) * movementSpeed, velocity.y,
                                      sinf(newMovementYaw) * movementSpeed);

        if (space && Desync.Enabled) {
            input->isJumping = false;
            Minecraft::ClientInstance->SetTimer(DesyncTimer.Value);
            RakPeerHooks::Ticking = true;
            RakPeerHooks::MsToSkip = DesyncMilliseconds.Value;
            RakPeerHooks::Visualize = nsm->VisualizeSetting.Enabled;
            RakPeerHooks::RenderCurrentAndLastPos = nsm->RenderCurrentAndLastPosSetting.Enabled;
            wasDesyncing = true;
        } else if (Desync.Enabled) {
            RakPeerHooks::Ticking = false;
            RakPeerHooks::MsToSkip = 0;
            wasDesyncing = false;
        }

        player->GetStateVectorComponent()->velocity = newVelocity;
        break;
    }
        case Autism: {
        if (space && Desync.Enabled) {
            input->isJumping = false;
            Minecraft::ClientInstance->SetTimer(DesyncTimer.Value);
            RakPeerHooks::Ticking = true;
            RakPeerHooks::MsToSkip = DesyncMilliseconds.Value;
            RakPeerHooks::Visualize = nsm->VisualizeSetting.Enabled;
            RakPeerHooks::RenderCurrentAndLastPos = nsm->RenderCurrentAndLastPosSetting.Enabled;
            wasDesyncing = true;
        } else if (Desync.Enabled) {
            RakPeerHooks::Ticking = false;
            RakPeerHooks::MsToSkip = 0;
            wasDesyncing = false;
        }

            // Friction stuff
            if (player->isOnGround()) {
                friction = 1.f;
                lastKbTime = 0;
            } else
            {
                // if the ticks is higher than freeze tick and lower than end tick then dont set friction
                if (FreezeFriction.Enabled && (OffGroundTicks < FreezeFrictionTick.Value && OffGroundTicks > FreezeFrictionEnd.Value)) {
                    friction *= FrictionSetting.Value;
                } else if (!FreezeFriction.Enabled)
                {
                    friction *= FrictionSetting.Value;
                }
            }

        if ((lastKbTime != 0 && Misc::GetCurrentMs() - lastKbTime < 1000)) {
            friction = 1.f;

            // Change the movement yaw
            Vector3 velocity = player->GetStateVectorComponent()->velocity;
            float movementSpeed = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
            float movementYaw = atan2(velocity.z, velocity.x);
            float movementYawDegrees = movementYaw * (180.0f / IM_PI) - 90.f;
            float playerYawDegrees = player->GetActorRotationComponent()->yaw +
                                     Misc::GetRotationKeyOffset(true);
            float yawDifference = playerYawDegrees - movementYawDegrees;
            float yawDifferenceRadians = yawDifference * (IM_PI / 180.0f);
            float newMovementYaw = movementYaw + yawDifferenceRadians;

            Vector3 newVelocity = Vector3(cosf(newMovementYaw) * movementSpeed, velocity.y,
                                          sinf(newMovementYaw) * movementSpeed);
            player->GetStateVectorComponent()->velocity = newVelocity;
        } else {
            lastKbTime = 0;

            auto velocity = player->GetStateVectorComponent()->velocity;
            auto accel = 1 / (sqrt(pow(velocity.x, 2) + pow(velocity.z, 2)) / speedSetting);
            player->GetStateVectorComponent()->velocity = Vector3(velocity.x * accel * friction, velocity.y, velocity.z * accel * friction);

            velocity= player->GetStateVectorComponent()->velocity;
            float movementSpeed = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
            float movementYaw = atan2(velocity.z, velocity.x);
            float movementYawDegrees = movementYaw * (180.0f / IM_PI) - 90.f;
            float playerYawDegrees = player->GetActorRotationComponent()->yaw +
                                     Misc::GetRotationKeyOffset(true);
            float yawDifference = playerYawDegrees - movementYawDegrees;
            float yawDifferenceRadians = yawDifference * (IM_PI / 180.0f);
            float newMovementYaw = movementYaw + yawDifferenceRadians;

            Vector3 newVelocity = Vector3(cosf(newMovementYaw) * movementSpeed, velocity.y,
                                          sinf(newMovementYaw) * movementSpeed);
            player->GetStateVectorComponent()->velocity = newVelocity;
        }


        break;
    }
#endif
    case Static: {
            if (space && Desync.Enabled) {
                input->isJumping = false;
                Minecraft::ClientInstance->SetTimer(DesyncTimer.Value);
                RakPeerHooks::Ticking = true;
                RakPeerHooks::MsToSkip = DesyncMilliseconds.Value;
                RakPeerHooks::Visualize = nsm->VisualizeSetting.Enabled;
                RakPeerHooks::RenderCurrentAndLastPos = nsm->RenderCurrentAndLastPosSetting.Enabled;
                wasDesyncing = true;
            } else if (Desync.Enabled) {
                RakPeerHooks::Ticking = false;
                RakPeerHooks::MsToSkip = 0;
                wasDesyncing = false;
            }

            // Friction stuff
            if (!VanillaFriction.Enabled) {
                if (player->isOnGround()) {
                    friction = 1.f;
                    lastKbTime = 0;
                } else {
                    // if the ticks is higher than freeze tick and lower than end tick then dont set friction
                    bool isFreezeTick =
                            OffGroundTicks > FreezeFrictionTick.Value && OffGroundTicks < FreezeFrictionEnd.Value;


                    if (FreezeFriction.Enabled && !isFreezeTick) {
                        friction *= FrictionSetting.Value;
                    } else if (!FreezeFriction.Enabled) {
                        friction *= FrictionSetting.Value;
                    } else if (isFreezeTick) {
                        friction *= 0.99f;
                    }
                }
            } else {
                if (player->isOnGround()) {
                    friction = 1.f;
                    lastKbTime = 0;
                } else {

                    bool isFreezeTick =
                            OffGroundTicks >= FreezeFrictionTick.Value && OffGroundTicks <= FreezeFrictionEnd.Value;

                    if (!FreezeFriction.Enabled)
                    {
                        friction = friction * 0.98f - 0.02f;
                        if (friction < 0) friction = 0;
                    } else if (FreezeFriction.Enabled && isFreezeTick) {
                        //Logger::Write("Speed", "frozen", Logger::LogType::Debug);
                        friction *= 0.99f;
                    } else {
                        friction = friction * 0.98f - 0.02f;
                        if (friction < 0) friction = 0;
                    }

                    /*Logger::Write("Speed", "FreezeFriction: " + std::to_string(FreezeFriction.Enabled &&
                        (OffGroundTicks < FreezeFrictionTick.Value && OffGroundTicks > FreezeFrictionEnd.Value)) + " " + std::to_string(OffGroundTicks) + " fric: " + std::to_string(friction), Logger::LogType::Debug);*/
                }
            }


            if (!DamageStrafe.Enabled && (Test.Enabled || (lastKbTime != 0 && Misc::GetCurrentMs() - lastKbTime < 1000))) {
                if (!Test.Enabled)
                    friction = 1.f;

                // Change the movement yaw
                Vector3 velocity = player->GetStateVectorComponent()->velocity;
                float movementSpeed = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);
                float movementYaw = atan2(velocity.z, velocity.x);
                float movementYawDegrees = movementYaw * (180.0f / IM_PI) - 90.f;
                float playerYawDegrees = player->GetActorRotationComponent()->yaw +
                                         Misc::GetRotationKeyOffset(true);
                float yawDifference = playerYawDegrees - movementYawDegrees;
                float yawDifferenceRadians = yawDifference * (IM_PI / 180.0f);
                float newMovementYaw = movementYaw + yawDifferenceRadians;

                Vector3 newVelocity = Vector3(cosf(newMovementYaw) * movementSpeed, velocity.y,
                                              sinf(newMovementYaw) * movementSpeed);
                player->GetStateVectorComponent()->velocity = newVelocity;
            } else {
                lastKbTime = 0;

                auto velocity = player->GetStateVectorComponent()->velocity;
                static auto lastSpeed = 1.f;

                if (player->isOnGround()) {
                    lastSpeed = 1;
                }

                float movementYaw = atan2(velocity.z, velocity.x);
                float movementYawDegrees = movementYaw * (180.0f / IM_PI) - 90.f;
                float rotation_key_offset = Misc::GetRotationKeyOffset(true);
                float playerYawDegrees = player->GetActorRotationComponent()->yaw +
                                         rotation_key_offset;

                if (movementYawDegrees > 180.f) movementYawDegrees -= 360.f;
                if (movementYawDegrees < -180.f) movementYawDegrees += 360.f;

                if (playerYawDegrees > 180.f) playerYawDegrees -= 360.f;
                if (playerYawDegrees < -180.f) playerYawDegrees += 360.f;

                float yawDifference = calculateShortestAngleDifference(playerYawDegrees + 180, movementYawDegrees + 180);

                float strafeDiff = (instantDirection ? 1 : 1 - (yawDifference / 180));

                strafeDiff = 1 - ((1 - strafeDiff) * StaticStrafeSlowdown.Value);

                if (strafeDiff < 0) strafeDiff = 0;
                if (strafeDiff > 0.98) strafeDiff = 1;

                float speed = ((!instantDirection ? (player->isOnGround() ? speedSetting : speedSetting * lastSpeed) : speedSetting) * friction) * strafeDiff;

                Vector2 motion = Misc::GetMotion(player->GetActorRotationComponent()->yaw, speed, strafeSetting);
                player->GetStateVectorComponent()->velocity = Vector3(motion.x, velocity.y, motion.y);
                lastSpeed *= strafeDiff;;
            }


        break;
        }
    }

    if (OffGroundTicks == BoostTick.Value && Boost.Enabled) {
        Vector2 motion = Misc::GetMotion(player->GetActorRotationComponent()->yaw, BoostSpeed.Value);
        player->GetStateVectorComponent()->velocity = Vector3(motion.x, player->GetStateVectorComponent()->velocity.y, motion.y);
    }
    if (Misc::IsUsingMovementKeys()) {
        float roundedHeight = LowHopHeight.Value;

        if (LowHopSetting.Enabled) {
            // If LowHopHeight setting is within 0.01 of 0.42, use player->jumpFromGround()
            if (LowHopBoost.Enabled) {
                player->jumpFromGround();
            }

            if (player->isOnGround()) {
                player->GetStateVectorComponent()->velocity.y = LowHopHeight.Value;
            }

        }
    }
}

void Speed::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (packet->getId() == (int) PacketID::SetEntityMotion) {
        auto sem = (SetEntityMotionPacket *) packet.get();
        if (sem->RuntimeID == Minecraft::ClientInstance->ThePlayer()->getRuntimeIDComponent()->runtimeID) {
            // Check if DamageBoost is enabled and if the player is using movement keys
            static auto damageBoost = ModuleManager::GetModule<DamageBoost>();
            if (damageBoost->Enabled && Misc::IsUsingMovementKeys()) {
                lastKbTime = Misc::GetCurrentMs();
                *cancel = true;
            }

            if (StaticDamageBoost.Enabled) {
                staticDamageBoostInc = StaticDamageBoostAmount.Value;
                if (StaticDamageBoostMs.Value > 0) {
                    staticDamageBoostStartTime = Misc::GetCurrentMs();
                }
            }

            if (DamageTimer.Enabled) {
                damageTimerEndTime = static_cast<uint64_t>(Misc::GetCurrentMs()) + (static_cast<uint64_t>(DamageTimerTicks.Value) * 50);
                Logger::Write("Speed", "DamageTimer: setting endTime to end in " + std::to_string(DamageTimerTicks.Value) + " ticks (" + std::to_string(damageTimerEndTime - Misc::GetCurrentMs()) + "ms)", Logger::LogType::Debug);
            }
        }
    }

    if (packet->getId() == (int)PacketID::MobEffect)
    {
        auto mobEffect = std::reinterpret_pointer_cast<MobEffectPacket>(packet);

        if (mobEffect->mRuntimeId != player->getRuntimeIDComponent()->runtimeID) return;

        std::string effect = std::string(mobEffect->GetEffectName());
        std::string eventName = std::string(mobEffect->GetEventName());
        uint64_t time = mobEffect->mEffectDurationTicks * 50;

        if (mobEffect->mEventId == MobEffectPacket::Event::Add) {
            effectTimers[(EffectType)mobEffect->mEffectId] = Misc::GetCurrentMs() + time;
        } else if (mobEffect->mEventId == MobEffectPacket::Event::Remove) {
            effectTimers.erase((EffectType)mobEffect->mEffectId);
        }

        Logger::Write("Speed", "MobEffect: " + effect + " Event: " + eventName + " Time: " + std::to_string(time), Logger::LogType::Debug);

    }
}

void Speed::OnPacket(class LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    static auto scaffold = ModuleManager::GetModule<Scaffold>();

    if (DisableWhileScaffolding.Enabled && scaffold && scaffold->Enabled) {
        return;
    }

    if (packet->getId() == (int) PacketID::PlayerAuthInput) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        auto paip = (PlayerAuthInputPacket *) packet;

        if (Misc::IsUsingMovementKeys()) paip->inputFlags |= AuthInputAction::JUMPING | AuthInputAction::WANT_UP | AuthInputAction::JUMP_DOWN | AuthInputAction::NORTH_JUMP;
        if (!player->isOnGround() && player->wasOnGround() && Misc::IsUsingMovementKeys()) {
            paip->inputFlags |= AuthInputAction::START_JUMPING;
        }

        if (RotateSetting.Enabled && Misc::IsUsingMovementKeys()) {
            Vector3 pos = *player->getPos();
            Vector3 oldPos = *player->getPosPrev();
            Vector2 rotations = Misc::GetRotations(&oldPos, &pos);
            paip->rotation.y = rotations.y;

            // We will not set the pitch because it will cause the player to look up and down
            paip->headYaw = rotations.y;

            // Remove AuthInputAction::LEFT, AuthInputAction::RIGHT, AuthInputAction::DOWN and add AuthInputAction::UP
            paip->inputFlags &= ~(AuthInputAction::LEFT | AuthInputAction::RIGHT | AuthInputAction::DOWN);
            paip->inputFlags |= AuthInputAction::UP;

            // Set the analog move vector to be the old moveVector
            paip->analogMoveVector = paip->moveVector;

            // Set the motion vector
            paip->moveVector = Vector2(0.f, 1.f);

        }
    }
}

