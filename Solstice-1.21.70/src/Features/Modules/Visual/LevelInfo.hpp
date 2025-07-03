#pragma once
#include <Features/Modules/Module.hpp>
//
// Created by vastrakai on 7/2/2024.
//


class LevelInfo : public ModuleBase<LevelInfo>
{
public:
    BoolSetting mShowFPS = BoolSetting("Show FPS", "Show the current FPS.", true);
    BoolSetting mShowPing = BoolSetting("Show Ping", "Show the current ping.", true);
    BoolSetting mShowBPS = BoolSetting("Show BPS", "Show the current BPS.", true);
    BoolSetting mShowXYZ = BoolSetting("Show XYZ", "Show the current XYZ coordinates.", true);
    BoolSetting mShowArrows = BoolSetting("Show Arrows", "Show the number of arrows in your inventory.", false);
    BoolSetting mShowEnderPearls = BoolSetting("Show Ender Pearls", "Show the number of ender pearls in your inventory.", false);
    BoolSetting mShowSpells = BoolSetting("Show Spells", "Show the spells in your inventory.", false);
    BoolSetting mShowHealthSpells = BoolSetting("Show Health Spells", "Show the number of health spells in your inventory.", false);
    BoolSetting mShowSpeedSpells = BoolSetting("Show Speed Spells", "Show the number of speed spells in your inventory.", false);
    BoolSetting mShowFireTrailSpells = BoolSetting("Show Fire Trail Spells", "Show the number of Fire Trail spells in your inventory.", false);
    BoolSetting mShowKicksAmount = BoolSetting("Kicks Counter", "Show the number of times you have been kicked (useful for avoiding bans in Flareon v2).", false);

    LevelInfo() : ModuleBase("LevelInfo", "Display general information.", ModuleCategory::Visual, 0, false) {
        addSetting(&mShowFPS);
        addSetting(&mShowPing);
        addSetting(&mShowBPS);
        addSetting(&mShowXYZ);
        addSetting(&mShowArrows);
        addSetting(&mShowEnderPearls);
        addSettings(&mShowSpells, &mShowHealthSpells, &mShowSpeedSpells, &mShowFireTrailSpells);
        addSetting(&mShowKicksAmount);

        VISIBILITY_CONDITION(mShowHealthSpells, mShowSpells.mValue);
        VISIBILITY_CONDITION(mShowSpeedSpells, mShowSpells.mValue);
        VISIBILITY_CONDITION(mShowFireTrailSpells, mShowSpells.mValue);

        mNames = {
            {Lowercase, "levelinfo"},
            {LowercaseSpaced, "level info"},
            {Normal, "LevelInfo"},
            {NormalSpaced, "Level Info"}
        };
    }

    int mArrows = 0;
    int mPearls = 0;
    int mHealthSpells = 0;
    int mHearts = 0;
    int mSpeedSpells = 0;
    int mSeconds = 0;
    int mFireTrailSpells = 0;
    int mBlocks = 0;
    int mKicksAmount = 0;
    float mBps = 0.f;
    float mAveragedBps = 0.f;
    std::map<uint64_t, float> mBpsHistory;
    __int64 mPing = 0;
    __int64 mEventDelay = 0;

    int getPearlsAmount();
    int getArrowsAmount();
    int getSpellsAmount(int mSpellIndex); // 1 - health spells, 2 - speed spells, 3 - firetrail spells
    void calculateValue(int mSpellIndex); // 1 - hearts amount (health spell), 2 - speed potion seconds (speed spell), 3 - fire blocks (firetrails spell)
    void spellsUpdate();
    void onEnable() override;
    void onDisable() override;
    void onSendImmediateEvent(class SendImmediateEvent& event);
    void onPingUpdateEvent(class PingUpdateEvent& event);
    void onBaseTickEvent(class BaseTickEvent& event);
    void onRenderEvent(class RenderEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
};