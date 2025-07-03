#pragma once
//
// Created by vastrakai on 9/8/2024.
//


class AntiVoid : public ModuleBase<AntiVoid> {
public:
    NumberSetting mFallDistance = NumberSetting("Fall Distance", "The distance you can fall before being teleported back", 5, 1, 15, 1);
    BoolSetting mTeleport = BoolSetting("Teleport", "Teleport back when falling", true);
    BoolSetting mTpOnce = BoolSetting("Teleport Once", "Only teleport once", false);
    BoolSetting mToggleFreecam = BoolSetting("Toggle Freecam", "Toggle freecam when teleported", false);

    AntiVoid();

    std::vector<glm::vec3> mOnGroundPositions;
    bool mTeleported = false;
    bool mCanTeleport = true;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};