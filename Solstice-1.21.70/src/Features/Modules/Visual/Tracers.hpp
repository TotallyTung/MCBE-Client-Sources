#pragma once
//
// Created by vastrakai on 10/4/2024.
//

class Tracers : public ModuleBase<Tracers> {
public:
    enum class CenterPoint {
        Top,
        Center,
        Bottom
    };

    EnumSettingT<CenterPoint> mCenterPoint = EnumSettingT<CenterPoint>("Center Point", "The point to center the text on.", CenterPoint::Center, "Top", "Center", "Bottom");
    BoolSetting mRenderFilled = BoolSetting("Render Filled", "Whether or not to render the ESP filled.", true);
    BoolSetting mRenderLocal = BoolSetting("Render Local", "Whether or not to render the ESP on the local player.", false);
    BoolSetting mShowFriends = BoolSetting("Show Friends", "Whether or not to render the ESP on friends.", true);

    Tracers() : ModuleBase("Tracers", "Draws a line to every entity", ModuleCategory::Visual, 0, false) {
        addSetting(&mCenterPoint);
        addSetting(&mRenderFilled);
        addSetting(&mRenderLocal);
        addSetting(&mShowFriends);

        mNames = {
            {Lowercase, "tracers"},
            {LowercaseSpaced, "tracers"},
            {Normal, "Tracers"},
            {NormalSpaced, "Tracers"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
};