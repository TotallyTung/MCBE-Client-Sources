#pragma once
//
// Created by vastrakai on 9/14/2024.
//


class ViewModel : public ModuleBase<ViewModel>
{
public:
    NumberSetting mX = NumberSetting("X", "X translation", 0, -4, 4, 0.01);
    NumberSetting mY = NumberSetting("Y", "Y translation", 0, -4, 4, 0.01);
    NumberSetting mZ = NumberSetting("Z", "Z translation", 0, -4, 4, 0.01);

    ViewModel() : ModuleBase("ViewModel", "Changes the viewmodel of the player", ModuleCategory::Visual, 0, false)
    {
        addSettings(
            &mX,
            &mY,
            &mZ
        );

        mNames = {
            {Lowercase, "viewmodel"},
            {LowercaseSpaced, "view model"},
            {Normal, "ViewModel"},
            {NormalSpaced, "View Model"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onBobHurtEvent(class BobHurtEvent& event);
};