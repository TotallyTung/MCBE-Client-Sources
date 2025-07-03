#pragma once
#include <Features/Modules/Module.hpp>
//
// Created by vastrakai on 7/2/2024.
//


class Arraylist : public ModuleBase<Arraylist>
{
public:

    enum class BackgroundStyle {
        Opacity,
        Shadow,
        Both
    };

    enum class Display {
        Outline,
        Bar,
        Split,
        None
    };

    enum class ModuleVisibility {
        All,
        Bound,
    };

    EnumSettingT<BackgroundStyle> mBackground = EnumSettingT("Background", "Background style", BackgroundStyle::Shadow, "Opacity", "Shadow", "Both");
    NumberSetting mBackgroundOpacity = NumberSetting("Opacity", "The opacity of the background", 1.f, 0.0f, 1.f, 0.01f);
    NumberSetting mBackgroundValue = NumberSetting("Background Value", "The value of the background", 0.0f, 0.0f, 1.f, 0.01f);
    NumberSetting mBlurStrength = NumberSetting("Blur Strength", "The strength of the blur.", 0.f, 0.f, 10.f, 0.1f);
    EnumSettingT<Display> mDisplay = EnumSettingT("Display", "Outline style", Display::Split, "Outline", "Bar", "Split", "None");
    EnumSettingT<ModuleVisibility> mVisibility = EnumSettingT("Visibility", "Module visibility", ModuleVisibility::All, "All", "Bound");
    BoolSetting mRenderMode = BoolSetting("Render Mode", "Renders the module mode next to the module name", true);
    BoolSetting mGlow = BoolSetting("Glow", "Enables glow", true);
    NumberSetting mGlowStrength = NumberSetting("Glow Strength", "The strength of the glow", 1.f, 0.5f, 1.f, 0.1f);
    BoolSetting mBoldText = BoolSetting("Bold Text", "Makes the text bold", true);
    NumberSetting mFontSize = NumberSetting("Font Size", "The size of the font", 25.f, 10.f, 40.f, 0.01f);

    Arraylist() : ModuleBase("Arraylist", "Displays a list of modules", ModuleCategory::Visual, 0, true) {
        addSettings(
            //&mBackground,
            //&mBackgroundOpacity,
            //&mBackgroundValue,
            &mBlurStrength,
            &mDisplay,
            &mVisibility,
            &mRenderMode,
            &mGlow,
            &mGlowStrength
            //&mBoldText
            //&mFontSize
        );

        mNames = {
            {Lowercase, "arraylist"},
            {LowercaseSpaced, "array list"},
            {Normal, "Arraylist"},
            {NormalSpaced, "Array List"}
        };
    }
    void onEnable() override;
    void onDisable() override;

    void onRenderEvent(class RenderEvent& event);
    std::string getSettingDisplay() override {
          return mDisplay.mValues[mDisplay.as<int>()];
    }
};