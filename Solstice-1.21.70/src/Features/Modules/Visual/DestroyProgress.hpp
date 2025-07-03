#pragma once

#include <Features/Modules/Module.hpp>

class DestroyProgress : public ModuleBase<DestroyProgress>
{
public:
    enum class ColorMode {
        Default,
        Theme
    };

    EnumSettingT<ColorMode> mColorMode = EnumSettingT<ColorMode>("Color", "The color mode", ColorMode::Default, "Default", "Theme");
    NumberSetting mOpacity = NumberSetting("Opacity", "The opacity of box", 0.40f, 0.f, 1.f, 0.01f);
    BoolSetting mFilled = BoolSetting("Filled", "Fill box", true);

    DestroyProgress() : ModuleBase("DestroyProgress", "Render Destroy Progress", ModuleCategory::Visual, 0, false) {
        addSettings(&mColorMode, &mOpacity, &mFilled);

        mNames = {
            {Lowercase, "destroyprogress"},
            {LowercaseSpaced, "destroy progress"},
            {Normal, "DestroyProgress"},
            {NormalSpaced, "Destroy Progress"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
};