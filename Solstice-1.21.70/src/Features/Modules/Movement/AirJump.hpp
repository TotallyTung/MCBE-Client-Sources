//
// Created by alteik on 04/09/2024.
//
#pragma once

#include "Features/Modules/Module.hpp"

class AirJump : public ModuleBase<AirJump>
{
public:
    AirJump() : ModuleBase<AirJump>("AirJump", "allow u jump on air", ModuleCategory::Movement, 0, false) {
        mNames = {
                {Lowercase, "airjump"},
                {LowercaseSpaced, "air jump"},
                {Normal, "AirJump"},
                {NormalSpaced, "Air Jump"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};