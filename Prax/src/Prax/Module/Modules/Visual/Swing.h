#pragma once
//
// Created by vastrakai on 9/23/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>

class Swing : public ModuleBase<Swing> {
public:

    enum class AnimationModes {
        None,
        BlockHit,
        Spin,

    };

    NumberSetting SwingSpeed = NumberSetting("Swing Time", "Changes the swing speed (The time it takes to complete a swing)", 0, 30, 6, 1);
    BoolSetting FluxSwing = BoolSetting("Flux Swing", "Flux client styled swinging", true);
    BoolSetting CustomSwingAngle = BoolSetting("Custom Swing Angle", "Changes the swing angle", false);
    NumberSetting CustomSwingAngleValue = NumberSetting("Swing Angle", "The custom swing angle value", -360, 360, -80, 0.01, &CustomSwingAngle);
    ModeSetting AnimationMode = ModeSetting("Animation Mode", "The animation mode to use", {"None", "BlockHit", "Spin"}, 1);
    NumberSetting SpinSpeed = NumberSetting("Spin Speed", "The speed at which to spin", 1, 5, 1, 1);
    NumberSetting SwingDelay = NumberSetting("Swing Delay", "The delay between swings", 0, 20, 0, 0.01);
    BoolSetting NoSwitchAnimation = BoolSetting("No Switch Animation", "Disables the switch animation", false);

//    NumberSetting TestNumber = NumberSetting("Test Number", "OnRender Number", 0, 1, 0, 0.01);
    Swing() : ModuleBase("Swing", "Changes how you swing", "Visual", 0, false) {
        AddSetting(&SwingSpeed);
        AddSetting(&FluxSwing);
        AddSetting(&CustomSwingAngle);
        AddSetting(&CustomSwingAngleValue);
        AddSetting(&AnimationMode);
        AddSetting(&SpinSpeed);
        AddSetting(&SwingDelay);
        AddSetting(&NoSwitchAnimation);
//        AddSetting(&TestNumber);
    }

    void OnEnable() override;
    void OnTick() override;
    void OnDisable() override;

    static uint64_t LastSwingTime;
    static void *ogTapBytes[9];

    void OnActorSwing(bool* cancel);



};