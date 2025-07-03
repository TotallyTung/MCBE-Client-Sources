#pragma once
//
// Created by vastrakai on 7/12/2024.
//

#include <Features/Modules/Module.hpp>

class MotionBlur : public ModuleBase<MotionBlur> {
public:
    NumberSetting mMaxFrames = NumberSetting("Max Frames", "The maximum amount of frames to blur", 1, 1, 20, 1);
    NumberSetting mIntensity = NumberSetting("Intensity", "The intensity of the motion blur", 0.90f, 0.01f, 1.0f, 0.01f);

    MotionBlur() : ModuleBase<MotionBlur>("MotionBlur", "Applies a blur effect to the screen while moving", ModuleCategory::Visual, 0, false) {
        addSetting(&mMaxFrames);
        addSetting(&mIntensity);

        mNames = {
            {Lowercase, "motionblur"},
            {LowercaseSpaced, "motion blur"},
            {Normal, "MotionBlur"},
            {NormalSpaced, "Motion Blur"}
        };
     }

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
};