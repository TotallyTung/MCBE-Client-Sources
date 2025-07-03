#pragma once
//
// Created by vastrakai on 7/12/2024.
//

#include <Features/Modules/Module.hpp>

class MidclickAction : public ModuleBase<MidclickAction> {
public:
    BoolSetting mThrowPearls = BoolSetting("Throw Pearls", "Whether to throw an ender pearl when you middle click", false);
    BoolSetting mHotbarOnly = BoolSetting("Hotbar Only", "Whether to only throw pearls from the hotbar", false);
    BoolSetting mAddFriend = BoolSetting("Add Friend", "Whether to add the player you middle clicked as a friend", false);
    BoolSetting mSetNukerBlock = BoolSetting("Set Nuker Block", "Whether to specify block of nuker", false);
    MidclickAction() : ModuleBase("MidclickAction", "Performs an action when you middle click", ModuleCategory::Player, 0, false)
    {
        addSetting(&mThrowPearls);
        addSetting(&mHotbarOnly);
        addSetting(&mAddFriend);
        addSetting(&mSetNukerBlock);

        VISIBILITY_CONDITION(mHotbarOnly, mThrowPearls.mValue == true);

        mNames = {
              {Lowercase, "midclickaction"},
                {LowercaseSpaced, "midclick action"},
                {Normal, "MidclickAction"},
                {NormalSpaced, "Midclick Action"}
        };
    };

    bool mThrowNextTick = false;
    bool mRotateNextTick = false;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};