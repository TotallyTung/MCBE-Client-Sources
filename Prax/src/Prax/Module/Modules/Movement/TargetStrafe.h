//
// Created by vastrakai on 4/15/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class TargetStrafe : public ModuleBase<TargetStrafe> {
public:
    NumberSetting Radius = NumberSetting("Radius", "The radius of the circle to strafe around the target.", 0, 5, 3, 0.01);
    NumberSetting Range = NumberSetting("Range", "The range to start strafing.", 0, 10, 5, 0.01);
    NumberSetting SpacingRadius = NumberSetting("Spacing Radius", "The radius of the circle to strafe around the target.", 0, 5, 3, 0.01);
    BoolSetting AutoJump = BoolSetting("Auto Jump", "Automatically jumps when strafing.", true);
    BoolSetting SwitchDirections = BoolSetting("Switch Directions", "Switches directions every time you horizontally collide with a block.", true);
    BoolSetting RequireJump = BoolSetting("Hold Space", "Requires you to jump to start strafing.", false);
    BoolSetting SpeedOnly = BoolSetting("Speed Only", "Only strafes when you are moving.", false);

    TargetStrafe() : ModuleBase("TargetStrafe", "Strafes around the target", "Movement") {
        AddSetting(&Radius);
        AddSetting(&SpacingRadius);
        AddSetting(&Range);
        AddSetting(&AutoJump);
        AddSetting(&SwitchDirections);
        AddSetting(&RequireJump);
        AddSetting(&SpeedOnly);
    }

    void OnTick() override;

    void OnDisable() override;

    void strafe() const;
};