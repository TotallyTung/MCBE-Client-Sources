#pragma once
//
// 7/31/2024.
//

#include <Features/Modules/Module.hpp>



class Phase : public ModuleBase<Phase> {
public:
    enum class Mode {
        Horizontal,
        Vertical,
        Clip
    };
    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the module", Mode::Horizontal, "Horizontal", "Vertical", "Clip");
    NumberSetting mSpeed = NumberSetting("Speed", "The speed of the vertical phase", 1.f, 0.f, 10.f, 0.1f);
    BoolSetting mBlink = BoolSetting("Blink", "Blink when phasing", false);
    BoolSetting mDelay = BoolSetting("Delay", "Add delay", false);
    NumberSetting mDelayMs = NumberSetting("Delay MS", "The delay in ms to skip packets", 500, 0, 1000, 50);
    BoolSetting mTest = BoolSetting("Test", "test", false);
    Phase() : ModuleBase("Phase", "Allows you to phase through walls", ModuleCategory::Movement, 0, false) {
        addSettings(&mMode, &mSpeed, &mBlink, &mDelay, &mDelayMs, &mTest);

        VISIBILITY_CONDITION(mSpeed, mMode.mValue == Mode::Vertical);
        VISIBILITY_CONDITION(mBlink, mMode.mValue == Mode::Horizontal);
        VISIBILITY_CONDITION(mDelay, mMode.mValue == Mode::Horizontal && mBlink.mValue);
        VISIBILITY_CONDITION(mDelayMs, mMode.mValue == Mode::Horizontal && mBlink.mValue && mDelay.mValue);
        VISIBILITY_CONDITION(mTest, mMode.mValue == Mode::Clip);

        mNames = {
            {Lowercase, "phase"},
            {LowercaseSpaced, "phase"},
            {Normal, "Phase"},
            {NormalSpaced, "Phase"}
        };
    }

    bool mMoving = false;
    bool mClip = false;

    uint64_t mLastSync = 0;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void onRunUpdateCycleEvent(class RunUpdateCycleEvent& event);
    std::vector<glm::ivec3> getCollidingBlocks();

    std::string getSettingDisplay() override {
        return mMode.mValues[mMode.as<int>()];
    }
};