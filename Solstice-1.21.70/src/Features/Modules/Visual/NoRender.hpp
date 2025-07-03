#pragma once
//
// Created by vastrakai on 11/14/2024.
//


class NoRender : public ModuleBase<NoRender> {
public:
    BoolSetting mNoBlockOverlay = BoolSetting("No Block Overlay", "Disables the block overlay from being stuck inside a block", true);
    BoolSetting mNoFire = BoolSetting("No Fire", "Disables the fire overlay", true);

    NoRender() : ModuleBase("NoRender", "Disables certain rendering elements", ModuleCategory::Visual, 0, false) {
        addSetting(&mNoBlockOverlay);
        addSetting(&mNoFire);

        mNames = {
            {Lowercase, "norender"},
            {LowercaseSpaced, "no render"},
            {Normal, "NoRender"},
            {NormalSpaced, "No Render"}
        };
    }

    static void patchOverlay(bool);

    void onEnable() override;
    void onDisable() override;
};