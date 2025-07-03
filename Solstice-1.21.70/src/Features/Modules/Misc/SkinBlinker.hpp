#pragma once
#include <Features/Events/PacketOutEvent.hpp>
//
// Created by Tozic on 9/16/2024.
//

class SkinBlinker : public ModuleBase<SkinBlinker> {
public:
    NumberSetting mDelay = NumberSetting("Delay", "The delay between skin changes.", 16.f, 0, 60.f, 1.f);

    SkinBlinker() : ModuleBase("SkinBlinker", "Randomly changes skins.", ModuleCategory::Misc, 0, false) {
        addSetting(&mDelay);

        mNames = {
            {Lowercase, "skinblinker"},
            {LowercaseSpaced, "skin blinker"},
            {Normal, "SkinBlinker"},
            {NormalSpaced, "Skin Blinker"},
        };
    }

    std::string mLastSkin = "";
    uint64_t mLastSkinChange = 0;

    std::string mId = "";
    std::string mPlayFabId = "";
    std::string mFullId = "";

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(PacketOutEvent& event);
};