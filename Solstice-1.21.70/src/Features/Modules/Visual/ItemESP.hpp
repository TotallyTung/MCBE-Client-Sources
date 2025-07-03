#pragma once
//
// Created by vastrakai on 8/7/2024.
//


class ItemESP : public ModuleBase<ItemESP> {
public:
    BoolSetting mDistanceLimited = BoolSetting("Distance Limited", "Whether to only show items within a certain distance", true);
    NumberSetting mDistance = NumberSetting("Distance", "The distance to show items within", 100.f, 0.f, 100.f, 1.f);
    BoolSetting mRenderFilled = BoolSetting("Render Filled", "Whether to render the boxes filled", false);
    BoolSetting mThemedColor = BoolSetting("Themed Color", "Whether to use the themed color", true);
    BoolSetting mShowNames = BoolSetting("Show Names", "Whether to show the item names", true);
    BoolSetting mDistanceScaledFont = BoolSetting("Distance Scaled Font", "Whether to scale the font based on distance", true);
    NumberSetting mFontSize = NumberSetting("Font Size", "The size of the font", 20, 1, 40, 0.01);;
    NumberSetting mScalingMultiplier = NumberSetting("Scaling Multiplier", "The multiplier to use for scaling the font", 1.25f, 0.f, 5.f, 0.01f);
    BoolSetting mHighlightUsefulItems = BoolSetting("Highlight Useful Items", "Toggle highlighting of useful items", true);
    BoolSetting mShowEnchant = BoolSetting("Show Enchant", "Whether to show the enchantments of the items", true);

    ItemESP () : ModuleBase("ItemESP", "Draws a box around items", ModuleCategory::Visual, 0, false) {
        addSettings(
            &mDistanceLimited,
            &mDistance,
            &mRenderFilled,
            &mThemedColor,
            &mShowNames,
            &mDistanceScaledFont,
            &mFontSize,
            &mScalingMultiplier,
            &mHighlightUsefulItems,
            &mShowEnchant
        );

        VISIBILITY_CONDITION(mDistance, mDistanceLimited.mValue);
        VISIBILITY_CONDITION(mFontSize, mShowNames.mValue);
        VISIBILITY_CONDITION(mDistanceScaledFont, mShowNames.mValue);
        VISIBILITY_CONDITION(mFontSize, mShowNames.mValue && !mDistanceScaledFont.mValue);
        VISIBILITY_CONDITION(mScalingMultiplier, mShowNames.mValue && mDistanceScaledFont.mValue);

        mNames = {
            {Lowercase, "itemesp"},
            {LowercaseSpaced, "item esp"},
            {Normal, "ItemESP"},
            {NormalSpaced, "Item ESP"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onRenderEvent(class RenderEvent& event);
};