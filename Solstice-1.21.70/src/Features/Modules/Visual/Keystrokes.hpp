#pragma once
//
// Created by vastrakai on 9/13/2024.
//


class Keystrokes : public ModuleBase<Keystrokes> {
public:
    BoolSetting mUseInterfaceColor = BoolSetting("Use Interface Color", "Whether or not to use the interface color", true);

    Keystrokes();

    std::unique_ptr<class HudElement> mElement;

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
};