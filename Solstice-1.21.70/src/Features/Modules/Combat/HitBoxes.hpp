#pragma once
//
// Created by dark on 02/19/2025.
//

#include <Features/Modules/Setting.hpp>
#include <Features/Modules/Module.hpp>

class HitBoxes : public ModuleBase<HitBoxes>
{
public:
    NumberSetting mHeight = NumberSetting("Height", "The height of hitbox", 1.8f, 1.8f, 5, 0.01);
    NumberSetting mWidth = NumberSetting("Width", "The width of hitbox", 0.6f, 0.6f, 5, 0.01);

    HitBoxes() : ModuleBase<HitBoxes>("Hit Boxes", "Expand the player's hit boxes", ModuleCategory::Combat, 0, false) {
        mNames = {
                {Lowercase, "hitboxes"},
                {LowercaseSpaced, "hit boxes"},
                {Normal, "HitBoxes"},
                {NormalSpaced, "Hit Boxes"}
        };

        addSettings(&mHeight, &mWidth);
    }
    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};