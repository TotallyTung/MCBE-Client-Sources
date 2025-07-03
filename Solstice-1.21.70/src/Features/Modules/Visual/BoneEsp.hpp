#pragma once
//
// Created by vastrakai on 9/13/2024.
//

struct BonePair {
    class Bone* mBone;
    class ActorPartModel* mPartModel;
};

class BoneEsp : public ModuleBase<BoneEsp>
{
public:
    enum class Style {
        Default
    };

    EnumSettingT<Style> mStyle = EnumSettingT<Style>("Style", "The style of the ESP.", Style::Default, "Default");
    BoolSetting mRenderLocal = BoolSetting("Render Local", "Whether or not to render the ESP on the local player.", false);
    BoolSetting mShowFriends = BoolSetting("Show Friends", "Whether or not to render the ESP on friends.", true);
    BoolSetting mDebug = BoolSetting("Debug", "Debugging information", false);

    BoneEsp() : ModuleBase("BoneEsp", "Draws a box around entities", ModuleCategory::Visual, 0, false) {
        addSetting(&mStyle);
        addSetting(&mRenderLocal);
        addSetting(&mShowFriends);
        addSetting(&mDebug);

        mNames = {
            {Lowercase, "boneesp"},
            {LowercaseSpaced, "bone esp"},
            {Normal, "BoneEsp"},
            {NormalSpaced, "Bone Esp"}
        };
    }

    // Bone holder [actor, bone, partModel]
    std::map<class Actor*, std::vector<BonePair>> mBoneMap;


    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
    void onBoneRenderEvent(class BoneRenderEvent& event);
};
