#pragma once

#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Psapi.h>
#include <memory>
#include <Prax/ExceptionHandler.h>

#include "../../Module.h"

class Speed : public ModuleBase<Speed> {
public:

    enum SpeedMode
    {
        Friction,
        Flareon,
        Static,
#ifndef __RELEASEFREE__
        UwU,
        Autism
#endif
    };

    enum DownMotionMode {
        Disabled,
        PredictVel,
        SetVel
    };

    // TODO: Grouped settings and dynamic settings
    ModeSetting SpeedModeSetting = ModeSetting("Mode", "The mode for speed", std::vector<std::string>{"Friction", "Flareon", "Static",
#ifndef __RELEASEFREE__
        "UwU", "Autism"
#endif
    }, 1);
    BoolSetting HiveMode = BoolSetting("Hive Netskip", "Hive netskip", false);
    BoolSetting StrafeSetting = BoolSetting("Strafe", "Allows you to strafe by holding A and D", true);
    BoolSetting UseStrafeSpeed = BoolSetting("Use Strafe Speed", "Strafe speed", false);
    NumberSetting StrafeSpeed = NumberSetting("Strafe Speed", "The speed to strafe at", 0, 30, 10.9f, 0.01, &UseStrafeSpeed);

    BoolSetting InstantDirection = BoolSetting("Instant Direction", "Instantly changes your direction", true);
    BoolSetting NoStrafeWhileScaffolding = BoolSetting("No Strafe While Scaffolding", "Disables strafe while scaffolding", true);
    BoolSetting DisableWhileScaffolding = BoolSetting("Disable While Scaffolding", "Disables the module while scaffolding", true);
    NumberSetting FrictionSetting = NumberSetting("Friction", "How much friction to apply", 0, 1, 0.98f, 0.01);
    BoolSetting VanillaFriction = BoolSetting("Vanilla Friction", "Uses the games friction logic insted of prax's (for static speed)", false);
    NumberSetting SpeedSetting = NumberSetting("Speed", "How fast you should go", 0, 30, 10.9f, 0.01);
    BoolSetting RandomizeSpeed = BoolSetting("Randomize Speed", "Slightly randomizes the speed", false);
    NumberSetting RandomMinSetting = NumberSetting("Random Minimum", "The minimum multiplier for the randomized speed", 0, 2, 0.8f, 0.01, &RandomizeSpeed);
    NumberSetting RandomMaxSetting = NumberSetting("Random Maximum", "The maximum multiplier for the randomized speed", 0, 2, 1.0f, 0.01, &RandomizeSpeed);
    BoolSetting RotateSetting = BoolSetting("Rotate", "Rotate to the direction you're moving\nModules like Aura and such will override this!", false);
    BoolSetting Desync = BoolSetting("Desync", "Lets you desync while holding space.", false);
    NumberSetting DesyncTimer = NumberSetting("Desync Timer", "The timer for desync to use", 0, 60, 30.f, 0.01, &Desync);
    NumberSetting DesyncSpeed = NumberSetting("Desync Speed", "The speed for desync to use", 0, 30, 10.9f, 0.01, &Desync);
    NumberSetting DesyncMilliseconds = NumberSetting("Desync Milliseconds", "The milliseconds for desync to use", 0, 1000, 0.f, 1, &Desync);
    BoolSetting UseSwiftnessBook = BoolSetting("Use Swiftness Book", "Uses a swiftness book to speed up while holding space", true);
    BoolSetting HoldSpace = BoolSetting("Hold Space", "Only speeds while you are holding space", false, &UseSwiftnessBook);
    BoolSetting SwiftnessJump = BoolSetting("Swiftness Jump", "Jumps while holding space", false, &UseSwiftnessBook);
    BoolSetting SwiftnessOffGround = BoolSetting("Swiftness Off-Ground", "Speeds while not on ground", false, &UseSwiftnessBook);
    NumberSetting SwiftnessSpeed = NumberSetting("Swiftness Speed", "Speed Value", 0, 1, 0.60f, 0.01, &UseSwiftnessBook);
    NumberSetting SwiftnessFriction = NumberSetting("Swiftness Friction", "Friction Value", 0, 1, 1.f, 0.01, &UseSwiftnessBook);
    NumberSetting SwiftnessTimer = NumberSetting("Swiftness Timer", "Speed Timer", 0, 40, 20.f, 0.01, &UseSwiftnessBook);

    BoolSetting LowHopSetting = BoolSetting("Jump", "Whether or not to jump", true);
    BoolSetting LowHopBoost = BoolSetting("Jump Boost", "emily castration.", false, &LowHopSetting);
    NumberSetting LowHopHeight = NumberSetting("Jump Height", "The height to jump at", 0, 1, 0.42f, 0.01, &LowHopSetting);
    BoolSetting ResetFriction = BoolSetting("Reset Friction", "Resets your friction at a certain tick", false);
    NumberSetting ResetFrictionTick = NumberSetting("Reset Friction Tick", "The tick to reset friction on", 0, 20, 11, 1, &ResetFriction);
    NumberSetting FrictionToReset = NumberSetting("Friction To Reset", "The friction to reset to", 0, 1, 0.98f, 0.01, &ResetFriction);
    BoolSetting ResetOnce = BoolSetting("Reset Once", "Resets the friction once until you're on ground again", false, &ResetFriction);
    BoolSetting FreezeFriction = BoolSetting("Freeze Friction", "Freezes your friction once a tick is passed", false);
    NumberSetting FreezeFrictionTick = NumberSetting("Freeze Tick Start", "The tick to freeze friction on", 0, 20, 11, 1, &FreezeFriction);
    NumberSetting FreezeFrictionEnd = NumberSetting("Freeze Tick End", "The tick to unfreeze friction on", 0, 20, 12, 1, &FreezeFriction);
    ModeSetting DownMotionMode = ModeSetting("Down Motion Mode", "The mode to use for down motion", {"Disabled", "Predict Vel", "Set Vel"}, 0);
    NumberSetting DownMotionTick = NumberSetting("Down Motion Tick", "The tick to apply down motion on", 0, 20, 7, 1);
    NumberSetting DownMotionPredict = NumberSetting("Down Motion Predict", "The amount of times to predict the next motion on that tick", 0, 20, 1, 1);
    BoolSetting SecondDownMotion = BoolSetting("Second Down Motion", "Applies a second down motion", false);
    NumberSetting SecondDownMotionTick = NumberSetting("Second Motion Tick", "The tick to apply the second down motion on", 0, 20, 7, 1, &SecondDownMotion);
    NumberSetting SecondDownMotionPredict = NumberSetting("Second Motion Predict", "The amount of times to predict the next motion on that tick", 0, 20, 1, 1, &SecondDownMotion);
    BoolSetting ResetMotion = BoolSetting("Reset Y Motion", "Resets your Y motion when you let go of movement keys or if you disable the module", false);
    BoolSetting Boost = BoolSetting("Boost", "Boosts your speed on a certain tick off-ground", false);
    NumberSetting BoostTick = NumberSetting("Boost Tick", "The tick to boost on", 0, 20, 7, 1, &Boost);
    NumberSetting BoostSpeed = NumberSetting("Boost Speed", "The speed to boost to", 0, 30, 10.9f, 0.01, &Boost);
    NumberSetting TimerBoost = NumberSetting("Timer Boost", "The timer to set when this module is enabled\nSet to -0.1 to not override timer", -0.1, 60, 28.30f, 0.01);

    BoolSetting Test = BoolSetting("Test", "Test", false);
    NumberSetting StaticStrafeSlowdown = NumberSetting("Static Strafe Slowdown", "The speed multiplier.", 0, 1, 1, 0.01);

    BoolSetting NoStep = BoolSetting("No Step", "Disables step", false);

    BoolSetting StaticDamageBoost = BoolSetting("Static Damage Boost", "Shrimply superior damage boost.", false);
    NumberSetting StaticDamageBoostAmount = NumberSetting("Damage Boost Amount", "The speed multiplier.", 1, 2, 1, 0.01, &StaticDamageBoost);
    NumberSetting StaticDamageBoostFalloff = NumberSetting("Damage Boost Falloff", "The falloff for the damage boost.", 0, 1, 0.07, 0.01, &StaticDamageBoost);
    NumberSetting StaticDamageBoostMinInc = NumberSetting("Damage Boost Min Inc", "The minimum increment for the damage boost.", 0, 1, 0.04, 0.01, &StaticDamageBoost);
    NumberSetting StaticDamageBoostMs = NumberSetting("Damage Boost Ms", "The max milliseconds for the damage boost. (Set to 0 to disable.)", 0, 5000, 0, 1, &StaticDamageBoost);

    BoolSetting DamageStrafe = BoolSetting("Damage Strafe", "Strafe while taking damage.", false, &StaticDamageBoost);
    NumberSetting BoostNeededToStrafe = NumberSetting("Boost Needed To Strafe", "The speed needed to strafe.", 0, 1, 0.14, 0.01, &DamageStrafe);

    BoolSetting DamageTimer = BoolSetting("Damage Timer", "Gives you a timer boost when you take damage.", false);
    NumberSetting DamageTimerTicks = NumberSetting("Damage Timer Ticks", "The amount of ticks to boost the timer for.", 0, 200, 140, 1, &DamageTimer);
    NumberSetting DamageTimerBoost = NumberSetting("Damage Timer Boost", "The timer.", 0, 200, 20, 0.01, &DamageTimer);
    BoolSetting DamageTimerCombine = BoolSetting("Damage Timer Combine", "Combines the normal timer boost with damage timer.", false, &DamageTimer);
    BoolSetting DamageTimerDesync = BoolSetting("Damage Timer Desync", "Desyncs while taking damage.", false, &DamageTimer);
    NumberSetting DamageTimerDesyncTicks = NumberSetting("DT Desync Ticks", "The amount of ticks to desync for.", 0, 200, 140, 1, &DamageTimerDesync);


    Speed() : ModuleBase("Speed", "Lets you move faster than intended", "Movement", 0, false) {
        SpeedModeSetting.Display = true;
        PacketPriority = Priority::VeryFirst;
        AddSetting(&SpeedModeSetting);
        AddSetting(&HiveMode);
        AddSetting(&StrafeSetting);
        AddSetting(&UseStrafeSpeed);
        AddSetting(&StrafeSpeed);
        AddSetting(&InstantDirection);
        AddSetting(&NoStrafeWhileScaffolding);
        AddSetting(&DisableWhileScaffolding);
        AddSetting(&FrictionSetting);
        AddSetting(&VanillaFriction);
        AddSetting(&SpeedSetting);
        AddSetting(&RandomizeSpeed);
        AddSetting(&RandomMinSetting);
        AddSetting(&RandomMaxSetting);
        AddSetting(&RotateSetting);
        AddSetting(&Desync);
        AddSetting(&DesyncTimer);

        AddSetting(&DesyncSpeed);
        AddSetting(&DesyncMilliseconds);
        AddSetting(&UseSwiftnessBook);
        AddSetting(&HoldSpace);
        AddSetting(&SwiftnessJump);
        AddSetting(&SwiftnessOffGround);
        AddSetting(&SwiftnessSpeed);
        AddSetting(&SwiftnessFriction);
        AddSetting(&SwiftnessTimer);
        AddSetting(&LowHopSetting);
        AddSetting(&LowHopBoost);
        AddSetting(&LowHopHeight);
        AddSetting(&ResetFriction);
        AddSetting(&ResetFrictionTick);
        AddSetting(&FrictionToReset);
        AddSetting(&ResetOnce);
        AddSetting(&FreezeFriction);
        AddSetting(&FreezeFrictionTick);
        AddSetting(&FreezeFrictionEnd);
        AddSetting(&DownMotionMode);
        AddSetting(&DownMotionTick);
        AddSetting(&DownMotionPredict);
        AddSetting(&SecondDownMotion);
        AddSetting(&SecondDownMotionTick);
        AddSetting(&SecondDownMotionPredict);
        AddSetting(&ResetMotion);
        AddSetting(&Boost);
        AddSetting(&BoostTick);
        AddSetting(&BoostSpeed);
        AddSetting(&TimerBoost);
        AddSetting(&NoStep);
        AddSetting(&Test);
        AddSetting(&StaticStrafeSlowdown);
        AddSetting(&StaticDamageBoost);
        AddSetting(&StaticDamageBoostAmount);
        AddSetting(&StaticDamageBoostFalloff);
        AddSetting(&StaticDamageBoostMinInc);
        AddSetting(&StaticDamageBoostMs);
        AddSetting(&DamageStrafe);
        AddSetting(&BoostNeededToStrafe);
#ifndef __RELEASEFREE__
        AddSetting(&DamageTimer);
        AddSetting(&DamageTimerTicks);
        AddSetting(&DamageTimerBoost);
        AddSetting(&DamageTimerCombine);
        AddSetting(&DamageTimerDesync);
        AddSetting(&DamageTimerDesyncTicks);
#endif
    }

    static int OffGroundTicks;

    // inline speed float array for a SprintHop mode
    static inline float Speeds[2] = { 0.0f, 0.0f };

    void OnEnable() override;
    void OnDisable() override;

    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;


    bool IsApplyingDamageTimer();
};