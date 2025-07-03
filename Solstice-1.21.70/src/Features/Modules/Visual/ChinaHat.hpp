#pragma once

//
// Created by alteik on 11/10/2024.
//

#include <Features/Modules/Module.hpp>

class ChinaHat : public ModuleBase<ChinaHat>
{
public:

    NumberSetting mSize = NumberSetting("Size", "How big the hat should be", 0.6f, 0.05f, 1.f, 0.01f);
    NumberSetting mGlowAmount = NumberSetting("GlowAmount", "How much the hat should glow", 10, 0, 100, 1);
    NumberSetting mOpacity = NumberSetting("Opacity", "The opacity of hat", 0.40f, 0.f, 1.f, 0.01f);

    ChinaHat() : ModuleBase("ChinaHat", "Render hat on ur head", ModuleCategory::Visual, 0, false) {

        addSetting(&mSize);
        addSetting(&mGlowAmount);
        addSetting(&mOpacity);

        mNames = {
                {Lowercase, "chinahat"},
                {LowercaseSpaced, "china hat"},
                {Normal, "ChinaHat"},
                {NormalSpaced, "China Hat"}
        };
    }

    int mTicks = 0;

    float toRadians(float deg);

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
};
