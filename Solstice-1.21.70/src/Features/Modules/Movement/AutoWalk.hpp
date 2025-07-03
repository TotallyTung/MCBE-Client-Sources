#pragma once
//
// Created by dark on 3/12/2025.
//

#include <Features/Modules/Module.hpp>

class AutoWalk : public ModuleBase<AutoWalk>
{
public:
    AutoWalk() : ModuleBase<AutoWalk>("AutoWalk", "Automatically walks to enemy", ModuleCategory::Movement, 0, false) {
        mNames = {
            {Lowercase, "autowalk"},
            {LowercaseSpaced, "auto walk"},
            {Normal, "AutoWalk"},
            {NormalSpaced, "Auto Walk"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onRenderEvent(class RenderEvent& event);
};