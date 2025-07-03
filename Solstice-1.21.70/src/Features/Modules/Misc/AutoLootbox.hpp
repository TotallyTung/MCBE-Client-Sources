//
// Created by alteik on 01/09/2024.
//
#pragma once
#include <Features/Modules/Module.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

class AutoLootbox : public ModuleBase<AutoLootbox> {
public:

    NumberSetting mRange = NumberSetting("Range", "The max range for the lootboxes", 5.f, 0.f, 10.f, 1.f);
    BoolSetting mRotate = BoolSetting("Rotate", "Enable or disable rotation", true);

    AutoLootbox() : ModuleBase("AutoLootbox", "Automatically breaks death treasures on The Hive", ModuleCategory::Player, 0, false) {

        addSettings(&mRange);
        addSettings(&mRotate);

        mNames = {
                {Lowercase, "autolootbox"},
                {LowercaseSpaced, "auto lootbox"},
                {Normal, "AutoLootbox"},
                {NormalSpaced, "Auto Lootbox"},
        };
    }

    static AABB mTargetedAABB;
    static bool mRotating;
    static uint64_t lastHit;

    void onEnable() override;
    void onDisable() override;
    void RotateToTreasure(class Actor* actor);
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
};