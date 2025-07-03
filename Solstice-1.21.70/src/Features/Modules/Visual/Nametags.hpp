#pragma once
//
// Created by vastrakai on 8/10/2024.
//

#include <Features/Events/BaseTickEvent.hpp>

class Nametags : public ModuleBase<Nametags>
{
public:
    enum class Style {
        Solstice
    };

    EnumSettingT<Style> mStyle = EnumSettingT<Style>("Style", "The style of the nametags.", Style::Solstice, "Solstice");
    BoolSetting mShowFriends = BoolSetting("Show Friends", "Whether or not to render the nametags on friends.", true);
    BoolSetting mRenderLocal = BoolSetting("Render Local", "Whether or not to render the nametags on the local player.", false);
    BoolSetting mDistanceScaledFont = BoolSetting("Distance Scaled Font", "Whether to scale the font based on distance", true);
    BoolSetting mShowIrcUsers = BoolSetting("Show IRC Users", "Whether to show IRC users in the nametags", true);
    NumberSetting mBlurStrength = NumberSetting("Blur Strength", "The strength of the blur.", 0.f, 0.f, 10.f, 0.1f);
    NumberSetting mFontSize = NumberSetting("Font Size", "The size of the font", 23, 1, 40, 0.01);;
    NumberSetting mScalingMultiplier = NumberSetting("Scaling Multiplier", "The multiplier to use for scaling the font", 0, 0.f, 5.f, 0.01f);
    NumberSetting mMinScale = NumberSetting("Minimum Scale", "The minimum scale to use for scaling the font", 20.f, 0.01f, 20.f, 0.01f);
    BoolSetting mShowBps = BoolSetting("Show BPS", "Show the BPS of the player", false);
    BoolSetting mAverageBps = BoolSetting("Average BPS", "Show the average BPS of the player", true);

    Nametags() : ModuleBase("Nametags", "Draws nametags above entities", ModuleCategory::Visual, 0, false) {
        addSettings(
            &mStyle,
            &mShowFriends,
            &mRenderLocal,
            //&mDistanceScaledFont,
            &mShowIrcUsers,
            &mBlurStrength,
            /*&mFontSize,
            &mScalingMultiplier,
            &mMinScale*/
            &mShowBps,
            &mAverageBps

        );

        VISIBILITY_CONDITION(mFontSize, !mDistanceScaledFont.mValue);
        VISIBILITY_CONDITION(mScalingMultiplier, mDistanceScaledFont.mValue);
        VISIBILITY_CONDITION(mMinScale, mDistanceScaledFont.mValue);
        VISIBILITY_CONDITION(mAverageBps, mShowBps.mValue);

        mNames = {
            {Lowercase, "nametags"},
            {LowercaseSpaced, "nametags"},
            {Normal, "Nametags"},
            {NormalSpaced, "Nametags"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(BaseTickEvent& event);
    void onRenderEvent(class RenderEvent& event);
    void onNametagRenderEvent(class NametagRenderEvent& event);
};