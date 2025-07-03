#pragma once
//
// Created by vastrakai on 7/19/2024.
//

#include <Features/Modules/Module.hpp>

class NoJumpDelay : public ModuleBase<NoJumpDelay> {
public:
    NoJumpDelay() : ModuleBase<NoJumpDelay>("NoJumpDelay", "Removes the delay between jumps", ModuleCategory::Movement, 0, false) {
        mNames = {
            {Lowercase, "nojumpdelay"},
            {LowercaseSpaced, "no jump delay"},
            {Normal, "NoJumpDelay"},
            {NormalSpaced, "No Jump Delay"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};