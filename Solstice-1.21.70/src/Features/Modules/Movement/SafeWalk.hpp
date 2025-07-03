#pragma once
//
// Created by vastrakai on 7/18/2024.
//

#include <Features/Modules/Module.hpp>

class SafeWalk : public ModuleBase<SafeWalk> {
public:
    SafeWalk() : ModuleBase("SafeWalk", "Prevents you from falling off blocks", ModuleCategory::Movement, 0, false)
    {
        mNames = {
            {Lowercase, "safewalk"},
            {LowercaseSpaced, "safe walk"},
            {Normal, "SafeWalk"},
            {NormalSpaced, "Safe Walk"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};