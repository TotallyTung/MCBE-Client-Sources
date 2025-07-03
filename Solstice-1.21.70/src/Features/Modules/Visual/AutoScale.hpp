#pragma once
#include <Features/Modules/Module.hpp>
//
// Created by vastrakai on 6/30/2024.
//




class AutoScale : public ModuleBase<AutoScale> {
public:
    NumberSetting mScaleSetting = NumberSetting("Scale", "The scale of the gui", 2.0f, 1.f, 4.0f, 0.1f);

    AutoScale() : ModuleBase("AutoScale", "Automatically sets the gui scale", ModuleCategory::Visual, 0, false) {
        addSetting(&mScaleSetting);

        mNames = {
            {Lowercase, "autoscale"},
            {LowercaseSpaced, "auto scale"},
            {Normal, "AutoScale"},
            {NormalSpaced, "Auto Scale"}
        };
    }

    float mOldScaleMultiplier{};
    glm::vec2 mOldScaledResolution{};
    float mOldGuiScale{};

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);

    std::string getSettingDisplay() override {
        return fmt::format("{:.1f}", mScaleSetting.mValue);
    }
};
