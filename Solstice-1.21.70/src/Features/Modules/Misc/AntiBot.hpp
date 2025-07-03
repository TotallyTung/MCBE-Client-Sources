#pragma once
//
// Created by vastrakai on 7/8/2024.
//
#include <Features/Modules/Module.hpp>

class AntiBot : public ModuleBase<AntiBot>
{
public:
    enum class Mode {
        Simple,
        Custom
    };

    enum class EntitylistMode {
        EnttView,
        RuntimeActorList
    };

    enum class ArmorMode
    {
        Full,
        OneElement
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the AntiBot module.", Mode::Simple, "Simple", "Custom");
    EnumSettingT<EntitylistMode> mEntitylistMode = EnumSettingT<EntitylistMode>("Entitylist Mode", "The mode of the entity list.\nEntt View: Use if you have any issues with actors not showing up\nRuntime Actor List: Use if you have stability issues", EntitylistMode::RuntimeActorList, "Entt View", "Level");
    BoolSetting mHitboxCheck = BoolSetting("Hitbox Check", "Whether or not to check the hitbox of the entity.", true);
    BoolSetting mPlayerCheck = BoolSetting("Player Check", "Whether or not to check if the entity is a player.", true);
    BoolSetting mInvisibleCheck = BoolSetting("Invisible Check", "Whether or not to check if the entity is invisible.", true);
    BoolSetting mNameCheck = BoolSetting("Name Check", "Whether or not to check if the entity name has more than one line.", true);
    BoolSetting mPlayerListCheck = BoolSetting("PlayerList Check", "Whether or not to check if the entity is in playerlist.", true);
    BoolSetting mHasArmorCheck = BoolSetting("Has Armor Check", "Whether or not to check if the entity has armor", false);
    EnumSettingT<ArmorMode> mArmorMode = EnumSettingT<ArmorMode>("Armor Mode", "The mode of the armor check.", ArmorMode::Full, "Full", "One Element");

    AntiBot() : ModuleBase("AntiBot", "Filters out bots from the entity list", ModuleCategory::Misc, 0, true) {
        addSettings(&mMode, &mEntitylistMode, &mHitboxCheck, &mPlayerCheck, &mInvisibleCheck, &mNameCheck, &mPlayerListCheck, &mHasArmorCheck, &mArmorMode);

        VISIBILITY_CONDITION(mHitboxCheck, mMode.mValue == Mode::Custom);
        VISIBILITY_CONDITION(mPlayerCheck, mMode.mValue == Mode::Custom);
        VISIBILITY_CONDITION(mInvisibleCheck, mMode.mValue == Mode::Custom);
        VISIBILITY_CONDITION(mNameCheck, mMode.mValue == Mode::Custom);
        VISIBILITY_CONDITION(mPlayerListCheck, mMode.mValue == Mode::Custom);
        VISIBILITY_CONDITION(mHasArmorCheck, mMode.mValue == Mode::Custom);
        VISIBILITY_CONDITION(mArmorMode, mMode.mValue == Mode::Custom && mHasArmorCheck.mValue);

        mNames = {
            {Lowercase, "antibot"},
            {LowercaseSpaced, "anti bot"},
            {Normal, "AntiBot"},
            {NormalSpaced, "Anti Bot"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    static std::vector<std::string> getPlayerNames();
    bool isBot(Actor* actor);
    bool hasArmor(Actor* actor);

    std::string getSettingDisplay() override {
        return mMode.mValues[mMode.as<int>()];
    }
};