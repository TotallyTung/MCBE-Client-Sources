#pragma once
//
// Created by dark on 9/23/2024.
//

#include <Features/Modules/Module.hpp>

class FastMine : public ModuleBase<FastMine> {
public:
    enum class Mode 
    {
        Hive,
        BDS,
        //Packet
    };

    EnumSettingT<Mode> mMode = EnumSettingT("Mode", "The mode of the fast mine", Mode::Hive, "Hive", "BDS");
    NumberSetting mDestroySpeed = NumberSetting("Destroy Speed", "The destroy speed for fast mine", 1, 0.01, 1, 0.01);
    BoolSetting mInfiniteDurability = BoolSetting("Infinite Durability", "Infinite durability for tools (may cause issues!)", false);
    FastMine() : ModuleBase("FastMine", "Increases your mining speed", ModuleCategory::Player, 0, false)
    {
        addSettings(&mMode, &mDestroySpeed, &mInfiniteDurability);

        VISIBILITY_CONDITION(mInfiniteDurability, mMode.mValue == Mode::Hive);

        mNames = {
              {Lowercase, "fastmine"},
                {LowercaseSpaced, "fast mine"},
                {Normal, "FastMine"},
                {NormalSpaced, "Fast Mine"}
        };
    };

    void onEnable();
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};