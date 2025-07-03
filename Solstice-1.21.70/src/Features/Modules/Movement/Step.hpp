#pragma once
//
// Created by vastrakai on 8/3/2024.
//


class Step : public ModuleBase<Step>
{
public:
    enum class Mode {
        Vanilla,
        Flareon,
        FlareonV2
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The style of which step height is applied.", Mode::Vanilla, "Vanilla", "Flareon", "FlareonV2");
    NumberSetting mStepHeight = NumberSetting("Step Height", "The max height to step up blocks", 0.50f, 0.0f, 5.f, 0.1f);

    Step() : ModuleBase<Step>("Step", "Automatically steps up blocks", ModuleCategory::Movement, 0, false) {
        addSetting(&mMode);
        addSetting(&mStepHeight);

        mNames = {
            {Lowercase, "step"},
            {LowercaseSpaced, "step"},
            {Normal, "Step"},
            {NormalSpaced, "Step"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};