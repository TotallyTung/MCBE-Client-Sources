#pragma once

#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include "../../Module.h"

class Aura : public ModuleBase<Aura> {
public:
    enum class AttackMode
    {
        Earliest,
        Synched
    };
    ModeSetting AuraModeSetting = ModeSetting("Mode", "The mode of the aura", {"Single", "Multi", "Switch"}, 2);
    ModeSetting RotateMode = ModeSetting("Rotate Mode", "The mode of the rotation", {"None", "Normal", "Flick"}, 1);
    ModeSetting AttackModeSetting = ModeSetting("Attack Mode", "The mode of the attack", {"Earliest", "Synched"}, 0);
    NumberSetting RotationCompensation = NumberSetting("Rotation Compensation", "Compensates for movement", 0, 10, 1, 0.01);
    NumberSetting TargetRange = NumberSetting("Target Range", "The max distance to target people from", 0, 20, 4.5f, 0.01);
    NumberSetting HitRange = NumberSetting("Hit Range", "The max distance to hit people from", 0, 20, 4.5f, 0.01);
    NumberSetting ApsSetting = NumberSetting("APS", "The amount of clicks per second", 0, 20, 14, 0.01);
    BoolSetting RandomizeAPS = BoolSetting("Randomize APS", "Randomizes the APS", false);
    NumberSetting RandomizeAPSMin = NumberSetting("Randomize APS Min", "The minimum APS to randomize", 0, 20, 10, 0.01, &RandomizeAPS);
    NumberSetting RandomizeAPSMax = NumberSetting("Randomize APS Max", "The maximum APS to randomize", 0, 20, 14, 0.01, &RandomizeAPS);
    NumberSetting HurtTimeAdjustment = NumberSetting("Hurt Time Adjustment", "The amount of Ticks to subtract from the hurt time", 0, 10, 1, 0.01);
    inline static BoolSetting ServerSide = BoolSetting("Server Side Rots", "Uses server side rotations, which won't make you strafe in the target's direction", true);;
    BoolSetting HurtTimeCompensationSetting = BoolSetting("Hurt Time", "Compensates for the hurt time", false);
    BoolSetting ThrowProjectilesSetting = BoolSetting("Throw Projectiles", "Throws projectiles at people", true);
    NumberSetting ThrowRate = NumberSetting("Throw Rate", "The rate at which to throw projectiles", 0, 20, 5, 0.01);
    BoolSetting Interact = BoolSetting("Interact", "Sends an InteractPacket as if you're looking at the entity", false);
    BoolSetting AutoSelect = BoolSetting("Auto Select", "Automatically select a sword to hit with", true);
    BoolSetting SpoofSlot = BoolSetting("Spoof Slot", "Spoofs the slot to the server", false);
    BoolSetting SpoofWhenScaffolding = BoolSetting("Spoof When Scaffolding", "Spoofs the slot to the server when scaffolding", false);
    BoolSetting HotbarOnly = BoolSetting("Hotbar Only", "Only selects swors from the hotbar", true);
    BoolSetting FistFriends = BoolSetting("Fist Friends", "Fist friends", false);
    BoolSetting AutoFireSword = BoolSetting("Auto Fire Sword", "the funny ya", false);
    BoolSetting FireSwordSpoof = BoolSetting("Fire Sword Spoof", "Spoofs the slot to the server when using a fire sword", false);
    BoolSetting Visualize = BoolSetting("Visualize", "Visualizes the aura with a circle around the target", true);
    BoolSetting VisualizeRainbow = BoolSetting("Visualize Rainbow", "Rainbow circle", false);
    NumberSetting VisualizeSpeed = NumberSetting("Visualize Speed", "The speed of the circle", 0, 10.f, 1.2f, 0.01);
    NumberSetting VisualizePoints = NumberSetting("Visualize Points", "The amount of points to draw", 0, 100, 3, 0.01);
    NumberSetting VisualizeSpin = NumberSetting("Visualize Spin", "Circle spin speed", 0, 30.f, 2.f, 0.01);
    BoolSetting Hive = BoolSetting("Hive", "Hive mode", true);

    Aura() : ModuleBase("Aura", "Automatically attacks people", "Combat", 'F', false) {
        AuraModeSetting.Display = true;
        AddSetting(&AuraModeSetting);
        AddSetting(&RotateMode);
        AddSetting(&AttackModeSetting);
        AddSetting(&RotationCompensation);
        AddSetting(&ServerSide);
        AddSetting(&TargetRange);
        AddSetting(&HitRange);
        AddSetting(&ApsSetting);
        AddSetting(&RandomizeAPS);
        AddSetting(&RandomizeAPSMin);
        AddSetting(&HurtTimeAdjustment);
        AddSetting(&HurtTimeCompensationSetting);
        AddSetting(&ThrowProjectilesSetting);
        AddSetting(&ThrowRate);
        AddSetting(&Interact);
        AddSetting(&AutoSelect);
        AddSetting(&HotbarOnly);
        AddSetting(&SpoofSlot);
        AddSetting(&SpoofWhenScaffolding);
        AddSetting(&FistFriends);
        AddSetting(&AutoFireSword);
        AddSetting(&FireSwordSpoof);
        AddSetting(&Visualize);
        AddSetting(&VisualizeRainbow);
        AddSetting(&VisualizeSpeed);
        AddSetting(&VisualizePoints);
        AddSetting(&VisualizeSpin);
        AddSetting(&Hive);
        DisableOnDimensionSwitch = true;
    }


    static bool HasTarget;
    static int OldSlot;
    static int SpoofedSlot;
    static uint64_t LastSwitchTime;
    static Actor* LastTargetedActor;
    static Actor* LastAttackedActor;
    static AABB LastTargetedAABB;
    static uint64_t LastBlockPlace;

    int GetArmorProtection(Actor* actor);
    bool IsFullDiamond(Actor* actor, bool noEnchants);
    bool IsPlayerProtected(Actor* actor);
    int GetSword(Actor* target);
    bool ShouldUseFireSword(Actor* target);
    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
    bool ThrowProjectiles();
    bool Tick();
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
    void OnRender() override;
    void RotateToEntity(Actor *actor);
    float GetDistanceFromClosestAABBPoint(Actor *actor, Vector3 *playerPos);
    Actor *CheckForObstructingActor(Actor *ogTarget, Vector3 *pos);


};