#pragma once

#include <Features/Modules/Module.hpp>

class PlaceHighlights : public ModuleBase<PlaceHighlights>
{
public:
    enum class ColorMode {
        Custom,
        Theme
    };

    EnumSettingT<ColorMode> mColorMode = EnumSettingT<ColorMode>("Color Mode", "The color mode", ColorMode::Custom, "Custom", "Theme");
    ColorSetting mBoxColor = ColorSetting("Box Color", "The color of the highlights", 0x15FF00);
    NumberSetting mDuration = NumberSetting("Duration", "The duration for render placed blocks", 2500, 500, 5000, 500);
    NumberSetting mMaxOpacity = NumberSetting("Max Opacity", "The max opacity of box", 0.70f, 0.20f, 1.f, 0.01f);
    BoolSetting mFilled = BoolSetting("Filled", "Fill box", true);

    PlaceHighlights() : ModuleBase("PlaceHighlights", "Render Place Highlights", ModuleCategory::Visual, 0, false) {
        addSettings(&mColorMode, &mBoxColor, &mDuration, &mMaxOpacity, &mFilled);

        VISIBILITY_CONDITION(mBoxColor, mColorMode.mValue == ColorMode::Custom);

        mNames = {
            {Lowercase, "placehighlights"},
            {LowercaseSpaced, "place highlights"},
            {Normal, "PlaceHighlights"},
            {NormalSpaced, "Place Highlights"}
        };
    }

    std::vector<glm::ivec3> mOffsetList = {
        glm::ivec3(0, -1, 0),
        glm::ivec3(0, 1, 0),
        glm::ivec3(0, 0, -1),
        glm::ivec3(0, 0, 1),
        glm::ivec3(-1, 0, 0),
        glm::ivec3(1, 0, 0),
    };

    std::unordered_map<glm::ivec3, uint64_t> mPlaceMap;

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};