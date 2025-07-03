#pragma once
//
// Created by vastrakai on 9/13/2024.
//


class FullBright : public ModuleBase<FullBright>
{
public:
    FullBright() : ModuleBase("FullBright", "Gives your game full brightness", ModuleCategory::Visual, 0, false) {
        mNames = {
            {Lowercase, "fullbright"},
            {LowercaseSpaced, "full bright"},
            {Normal, "FullBright"},
            {NormalSpaced, "Full Bright"}
        };

        gFeatureManager->mDispatcher->listen<RenderEvent, &FullBright::onRenderEvent>(this);
    }

    bool mFinishedAnimation = true;
    float mOldValue = 1.f;

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
};