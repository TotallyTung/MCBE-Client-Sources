#pragma once
//
// Created by vastrakai on 7/7/2024.
//
#include <Features/Modules/Module.hpp>


class ESP : public ModuleBase<ESP>
{
public:
    enum class Style {
        Style3D
    };

    EnumSettingT<Style> mStyle = EnumSettingT<Style>("Style", "The style of the ESP.", Style::Style3D, "3D");
    BoolSetting mRenderFilled = BoolSetting("Render Filled", "Whether or not to render the ESP filled.", true);
    BoolSetting mRenderLocal = BoolSetting("Render Local", "Whether or not to render the ESP on the local player.", false);
    BoolSetting mShowFriends = BoolSetting("Show Friends", "Whether or not to render the ESP on friends.", true);
    BoolSetting mDebug = BoolSetting("Debug", "Whether or not to display bots.", false);

    ESP() : ModuleBase("ESP", "Draws a box around entities", ModuleCategory::Visual, 0, false) {
        addSetting(&mStyle);
        addSetting(&mRenderFilled);
        addSetting(&mRenderLocal);
        addSetting(&mShowFriends);
        addSetting(&mDebug);

        mNames = {
            {Lowercase, "esp"},
            {LowercaseSpaced, "esp"},
            {Normal, "ESP"},
            {NormalSpaced, "ESP"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
};