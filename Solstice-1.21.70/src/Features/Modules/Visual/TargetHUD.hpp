#pragma once
#include <SDK/Minecraft/Actor/EntityId.hpp>

#include "HudEditor.hpp"
//
// Created by vastrakai on 8/4/2024.
//


class TargetHUD : public ModuleBase<TargetHUD> {
public:
    enum class Style {
        Solstice,
    };

    EnumSettingT<Style> mStyle = EnumSettingT("Style", "The style of the target HUD", Style::Solstice, "Solstice");
    NumberSetting mXOffset = NumberSetting("X Offset", "The X offset of the target HUD", 100, -400, 400, 1);
    NumberSetting mYOffset = NumberSetting("Y Offset", "The Y offset of the target HUD", 100, -400, 400, 1);
    NumberSetting mFontSize = NumberSetting("Font Size", "The size of the font", 20, 1, 40, 1);
    BoolSetting mHealthCalculation = BoolSetting("Health Calculation", "Calculate health", false);

    TargetHUD();


    struct TargetTextureHolder {
        ID3D11ShaderResourceView* texture = nullptr;
        bool loaded = false;
        EntityId associatedEntity = EntityId();
    };

    float mHealth = 0;
    float mMaxHealth = 0;
    float mLastHealth = 0;
    float mLastMaxHealth = 0;
    float mAbsorption = 0;
    float mMaxAbsorption = 0;
    float mLastAbsorption = 0;
    float mLastMaxAbsorption = 0;
    float mLerpedHealth = 0;
    float mLerpedAbsorption = 0;
    std::string mLastPlayerName = "";
    float mLastHurtTime = 0;
    float mHurtTime = 0;
    Actor* mLastTarget = nullptr;
    std::map<Actor*, TargetTextureHolder> mTargetTextures;
    constexpr static uint64_t cHurtTimeDuration = 500;

    // Health Calculation
    struct HealthInfo {
        float health = 20;
        float lastAbsorption = 0;
        float damage = 1;
    };
    std::map<std::string, HealthInfo> mHealths;
    uint64_t mLastHealTime = 0;

    std::unique_ptr<HudElement> mElement;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void validateTextures();
    void calculateHealths();
    ID3D11ShaderResourceView* getActorSkinTex(Actor* actor);
    void onRenderEvent(class RenderEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
};