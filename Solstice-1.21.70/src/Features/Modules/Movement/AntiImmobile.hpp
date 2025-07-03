#pragma once
//
// Created by vastrakai on 7/3/2024.
//

#include <Features/Modules/Module.hpp>



class AntiImmobile : public ModuleBase<AntiImmobile> {
public:
    enum class Mode {
        Normal,
        Clip
    };
    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the module", Mode::Normal, "Normal", "Clip");
    AntiImmobile() : ModuleBase("AntiImmobile", "Prevents you from being immobile, or clips you to the ground when immobile", ModuleCategory::Movement, 0, false) {
        addSetting(&mMode);

        mNames = {
            {Lowercase, "antiimmobile"},
            {LowercaseSpaced, "anti immobile"},
            {Normal, "AntiImmobile"},
            {NormalSpaced, "Anti Immobile"}
        };
    }

    uint64_t mLastDimensionChange = 0;
    uint64_t mLastTeleport = 0;


    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);

    std::string getSettingDisplay() override {
        return mMode.mValues[mMode.as<int>()];
    }
};