//
// Created by vastrakai on 10/17/2023.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/SDK/Game/Misc/Structs.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>

class InfiniteAura : public ModuleBase<InfiniteAura> {
public:
    ModeSetting Mode = ModeSetting("Mode", "The mode to use for targeting", { "Single" }, 0);
    NumberSetting APS = NumberSetting("APS", "The amount of attacks per second", 0, 20, 10, 1);
    NumberSetting AttackDistance = NumberSetting("Attack Distance", "The distance to attack entities from", 0, 10, 3.5, 1);
    NumberSetting EasingTicks = NumberSetting("Easing Speed", "The amount of ticks it takes to ease the position to the target", 0, 60, 40, 1);
    BoolSetting HurtTime = BoolSetting("Hurt Time", "Exclude entities that are in hurt time", true);
    NumberSetting HurtTimeCompensation = NumberSetting("Hurt Time Compensation", "The amount of ticks to compensate for hurt time", 0, 10, 0, 1);
    BoolSetting RaycastCheck = BoolSetting("Raycast Check", "Whether or not to check if the entity is visible from the server side position", true);
    BoolSetting SilentAcceptLagbacks = BoolSetting("Silent Accept Lagbacks", "Whether or not to accept lagbacks silently", true);

    InfiniteAura() : ModuleBase("InfiniteAura", "Automatically attacks entities around you with an infinite range", "Combat", 0, false) {
        AddSetting(&APS);
        AddSetting(&Mode);
        AddSetting(&AttackDistance);
        AddSetting(&EasingTicks);
        AddSetting(&HurtTime);
        AddSetting(&HurtTimeCompensation);
        AddSetting(&RaycastCheck);
        AddSetting(&SilentAcceptLagbacks);
        Mode.Display = true;
        DisableOnDimensionSwitch = true;
    }

    static bool HasTarget;
    static Vector3 LastPos;
    static Vector3 ServersidePos;
    static int EaseTicks;
    static __int64 nextHitTime;
    static Vector3 TargetPos;
    static __int64 LastLagbackTime;


    bool CanRaycastTo(Actor* actor);

    void OnTick() override;
    void OnEnable() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
    void OnRender() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;

};