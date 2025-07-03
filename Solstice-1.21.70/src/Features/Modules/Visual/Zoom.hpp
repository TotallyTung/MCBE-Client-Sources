#pragma once

//
// Created by alteik on 15/10/2024.
//

class Zoom : public ModuleBase<Zoom> {
public:

    NumberSetting mZoomValue = NumberSetting("Zoom Value", "How much to zoom in", 60.f, 10.f, 120.f, 1.f);
    BoolSetting mScroll = BoolSetting("Scroll", "Whether or not to change fov while scrolling mouse wheel", true);
    NumberSetting mScrollIncrement = NumberSetting("Scroll Increment", "The amount to zoom in and out", 8.0f, 0.1f, 10.0f, 0.1f);
    BoolSetting mSmooth = BoolSetting("Smooth Zoom", "Whether or not to zoom smooth", true);

    Zoom() : ModuleBase<Zoom>("Zoom", "Decreases your field of view", ModuleCategory::Visual, 0, false) {

        addSetting(&mZoomValue);
        addSetting(&mScroll);
        addSetting(&mScrollIncrement);
        addSetting(&mSmooth);

        VISIBILITY_CONDITION(mScrollIncrement, mScroll.mValue);

        mNames = {
            {Lowercase, "zoom"},
            {LowercaseSpaced, "zoom"},
            {Normal, "Zoom"},
            {NormalSpaced, "Zoom"}
        };

        mEnableWhileHeld = true;
    }

    float mPastFov = 0;
    float mCurrentValue = 0;

    void onEnable() override;
    void onDisable() override;
    void onMouseEvent(class MouseEvent& event);
    void onRenderEvent(class RenderEvent& event);
};
