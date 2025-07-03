#pragma once

//
// Created by alteik on 15/10/2024.
//

class Jetpack : public ModuleBase<Jetpack>
{
public:

    NumberSetting mVerticalSpeed = NumberSetting("Vertical Speed", "vertical fly speed", 1.0f, 0.5f, 10.0f, 0.01f);
    NumberSetting mHorizontalSpeed = NumberSetting("Horizontal Speed", "horizontal fly speed", 1.0f, 0.5f, 10.0f, 0.01f);
    BoolSetting mReset = BoolSetting("Reset On Disable", "Reset ur motion on disable", false);

    Jetpack() : ModuleBase<Jetpack>("Jetpack", "Lets you fly", ModuleCategory::Movement, 0, false) {

        addSettings(&mVerticalSpeed, &mHorizontalSpeed);
        addSetting(&mReset);

        mNames = {
            {Lowercase, "jetpack"},
            {LowercaseSpaced, "jetpack"},
            {Normal, "Jetpack"},
            {NormalSpaced, "Jetpack"}
        };

        mEnableWhileHeld = true;
    }

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};
