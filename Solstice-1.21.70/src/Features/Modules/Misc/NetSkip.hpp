#pragma once
//
// Created by vastrakai on 8/3/2024.
//


class NetSkip : public ModuleBase<NetSkip>
{
public:
    enum class Mode {
        Ticks,
        Milliseconds
    };
    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the delay", Mode::Ticks, "Ticks", "Milliseconds");

    // Ticks settings set
    BoolSetting mRandomizeTicks = BoolSetting("Randomize Ticks", "Randomizes the delay", false);
    NumberSetting mTicks = NumberSetting("Ticks", "The delay in ticks to skip packets", 1, 0, 100, 1);
    NumberSetting mTicksMin = NumberSetting("Ticks Min", "The minimum delay to randomize", 0, 0, 100, 1);
    NumberSetting mTicksMax = NumberSetting("Ticks Max", "The maximum delay to randomize", 100, 0, 100, 1);


    // Milliseconds settings set
    BoolSetting mRandomizeDelayMs = BoolSetting("Randomize Delay", "Randomizes the delay", false);
    NumberSetting mDelayMs = NumberSetting("Delay", "The delay in ms to skip packets", 101, 0, 1000, 1);
    NumberSetting mRandomizeMinMs = NumberSetting("Delay Min", "The minimum delay to randomize", 0, 0, 1000, 1);
    NumberSetting mRandomizeMaxMs = NumberSetting("Delay Max", "The maximum delay to randomize", 100, 0, 1000, 1);

    BoolSetting mDamageOnly = BoolSetting("Damage Only", "Only skip when taking damage", false);
    NumberSetting mDamageTime = NumberSetting("Damage Time", "The amount of time to keep skipping after taking damage (milliseconds)", 0, 0, 10000, 1);

    NetSkip() : ModuleBase<NetSkip>("NetSkip", "Skips packets", ModuleCategory::Misc, 0, false) {
        addSettings(
            &mMode,

            &mRandomizeTicks,
            &mTicks,
            &mTicksMin,
            &mTicksMax,

            &mRandomizeDelayMs,
            &mDelayMs,
            &mRandomizeMinMs,
            &mRandomizeMaxMs,

            &mDamageOnly,
            &mDamageTime
        );

        VISIBILITY_CONDITION(mTicks, mMode.mValue == Mode::Ticks);
        VISIBILITY_CONDITION(mTicksMin, mMode.mValue == Mode::Ticks && mRandomizeTicks.mValue);
        VISIBILITY_CONDITION(mTicksMax, mMode.mValue == Mode::Ticks && mRandomizeTicks.mValue);
        VISIBILITY_CONDITION(mRandomizeTicks, mMode.mValue == Mode::Ticks);

        VISIBILITY_CONDITION(mDelayMs, mMode.mValue == Mode::Milliseconds);
        VISIBILITY_CONDITION(mRandomizeMinMs, mMode.mValue == Mode::Milliseconds && mRandomizeDelayMs.mValue);
        VISIBILITY_CONDITION(mRandomizeMaxMs, mMode.mValue == Mode::Milliseconds && mRandomizeDelayMs.mValue);
        VISIBILITY_CONDITION(mRandomizeDelayMs, mMode.mValue == Mode::Milliseconds);

        VISIBILITY_CONDITION(mDamageTime, mDamageOnly.mValue);

        mNames = {
            {Lowercase, "netskip"},
            {LowercaseSpaced, "net skip"},
            {Normal, "NetSkip"},
            {NormalSpaced, "Net Skip"}
        };
    }

    int mCurrentTick = 0;
    int mTickDelay = 0;
    int64_t mLastDamage = 0;

    void onEnable() override;
    void onDisable() override;
    void onRunUpdateCycleEvent(class RunUpdateCycleEvent& event);
    void onPacketInEvent(class PacketInEvent& event);

    std::string getSettingDisplay() override {
        return mMode.mValues[mMode.as<int>()];
    }
};