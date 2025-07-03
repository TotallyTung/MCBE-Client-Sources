#pragma once
//
// Created by alteik on 04/09/2024.
//

#include <Features/Modules/Setting.hpp>
#include <Features/Modules/Module.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

class AutoClicker : public ModuleBase<AutoClicker>
{
public:
    enum class ClickMode
    {
        Left,
        Right,
        Both
    };

    EnumSettingT<ClickMode> mClickMode = EnumSettingT<ClickMode>("Click Mode", "The click mode", ClickMode::Left, "Left", "Right", "Both");
    BoolSetting mHold = BoolSetting("Hold", "Only click when holding", false);
    BoolSetting mRandomizeCPS = BoolSetting("Randomize CPS", "Randomize the CPS", false);
    BoolSetting mWeaponsOnly = BoolSetting("Weapons Only", "Only click when holding a weapon", false);
    NumberSetting mCPS = NumberSetting("CPS", "The amount of times to click every second.", 16, 1, 60, 1);
    NumberSetting mRandomCPSMin = NumberSetting("CPS Min", "The minimum amount of times to click every second.", 10, 1, 60, 1);
    NumberSetting mRandomCPSMax = NumberSetting("CPS Max", "The maximum amount of times to click every second.", 20, 1, 60, 1);
    BoolSetting mAllowBlockBreaking = BoolSetting("Allow Block Breaking", "Allow block breaking", false);

    AutoClicker() : ModuleBase<AutoClicker>("AutoClicker", "Automatically clicks for you", ModuleCategory::Combat, 0, false) {
        addSettings(
            &mClickMode,
            &mRandomizeCPS,
            &mCPS,
            &mRandomCPSMin,
            &mRandomCPSMax,
            &mHold,
            &mWeaponsOnly,
            &mAllowBlockBreaking
        );

        VISIBILITY_CONDITION(mCPS, !mRandomizeCPS.mValue);
        VISIBILITY_CONDITION(mRandomCPSMin, mRandomizeCPS.mValue);
        VISIBILITY_CONDITION(mRandomCPSMax, mRandomizeCPS.mValue);
        VISIBILITY_CONDITION(mAllowBlockBreaking, mClickMode.mValue == ClickMode::Left);

        mNames = {
                {Lowercase, "autoclicker"},
                {LowercaseSpaced, "auto clicker"},
                {Normal, "AutoClicker"},
                {NormalSpaced, "Auto Clicker"}
        };
    }

    int mCurrentCPS = 10;

    void randomizeCPS()
    {
        mCurrentCPS = getCPS();
    }

    int getCPS()
    {
        if (mRandomizeCPS.mValue)
        {
            return MathUtils::random(mRandomCPSMin.as<int>(), mRandomCPSMax.as<int>());
        }

        return mCPS.mValue;
    }

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
};