#pragma once
//
// Created by vastrakai on 7/23/2024.
//

#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Modules/Module.hpp>
#include <Features/Events/BaseTickEvent.hpp>

class RobloxCamera : public ModuleBase<RobloxCamera> {
public:
    NumberSetting mRadius = NumberSetting("Radius", "The radius of the camera", 4.0f, 1.0f, 20.0f, 1.0f);
    BoolSetting mScroll = BoolSetting("Scroll", "Scroll to zoom in and out while holding control", true);
    NumberSetting mScrollIncrement = NumberSetting("Scroll Increment", "The amount to zoom in and out", 1.0f, 0.1f, 10.0f, 0.1f);
    BoolSetting mNoClip = BoolSetting("No Clipping", "No clip camera through blocks", false);

    RobloxCamera() : ModuleBase("RobloxCamera", "Change the camera to be like Roblox's camera", ModuleCategory::Visual, 0, false) {
        addSetting(&mRadius);
        addSetting(&mScroll);
        addSetting(&mScrollIncrement);
        addSetting(&mNoClip);

        VISIBILITY_CONDITION(mScrollIncrement, mScroll.mValue);

        mNames = {
            {Lowercase, "robloxcamera"},
            {LowercaseSpaced, "roblox camera"},
            {Normal, "RobloxCamera"},
            {NormalSpaced, "Roblox Camera"}
        };

        gFeatureManager->mDispatcher->listen<BaseTickInitEvent, &RobloxCamera::onBaseTickInitEvent>(this);
        gFeatureManager->mDispatcher->listen<ModuleStateChangeEvent, &RobloxCamera::onModuleStateChangeEvent>(this);
    }

    void onModuleStateChangeEvent(ModuleStateChangeEvent& event);

    void onBaseTickInitEvent(BaseTickInitEvent& event);

    bool mHasComponents = false;
    float mCurrentDistance = 4.f;

    void onEnable() override;
    void onDisable() override;
    void onActorRenderEvent(class ActorRenderEvent& event);
    void onMouseEvent(class MouseEvent& event);
    void onBaseTickEvent(class BaseTickEvent& event);
    void onLookInputEvent(class LookInputEvent& event);
};