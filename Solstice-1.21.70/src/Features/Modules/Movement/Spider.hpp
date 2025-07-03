//
// Created by ssi on 8/17/2024.
//

#pragma once
#include <Features/Modules/Module.hpp>
#include <Features/Modules/Setting.hpp>

class Spider : public ModuleBase<Spider> {
public:
    enum class Mode {
        Clip,
#ifdef __PRIVATE_BUILD__
        Flareon
#endif
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "Choose the climbing mode to use", Mode::Clip, "Clip"
#ifdef __PRIVATE_BUILD__
    ,"Flareon");
#else
    );
#endif
    NumberSetting mSpeed = NumberSetting("Speed", "Adjust the climbing speed", 2.50, 1, 5, 0.01);
    BoolSetting mOnGroundOnly = BoolSetting("OnGround only", "uses spider only if u on ground", false);

    Spider() : ModuleBase("Spider", "Allows you to climb up walls", ModuleCategory::Movement, 0, false) {
        addSetting(&mMode);
        addSetting(&mSpeed);
        addSetting(&mOnGroundOnly);

        mNames = {
            {Lowercase, "spider"},
            {LowercaseSpaced, "spider"},
            {Normal, "Spider"},
            {NormalSpaced, "Spider"},
        };
    }

    float mPosY = 0.f;
    bool mWasCollided = false;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};