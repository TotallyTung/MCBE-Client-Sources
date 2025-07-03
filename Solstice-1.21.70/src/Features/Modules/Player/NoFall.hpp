#pragma once
//
// Created by vastrakai on 8/30/2024.
//

#include <Features/Modules/Module.hpp>

class NoFall : public ModuleBase<NoFall> {
public:
    enum class Mode {
        Sentinel,
        BDS,
        OnGround,
        Lifeboat
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the nofall", Mode::Sentinel, "Sentinel", "BDS", "OnGround", "Lifeboat");

    NoFall() : ModuleBase("NoFall", "Prevents fall damage", ModuleCategory::Player, 0, false)
    {
        addSetting(&mMode);

        mNames = {
            {Lowercase, "nofall"},
            {LowercaseSpaced, "no fall"},
            {Normal, "NoFall"},
            {NormalSpaced, "No Fall"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onPacketOutEvent(class PacketOutEvent& event);

    std::string getSettingDisplay() override {
        return mMode.mValues[mMode.as<int>()];
    }
};