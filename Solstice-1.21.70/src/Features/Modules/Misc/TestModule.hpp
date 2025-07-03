//
// Created by vastrakai on 6/28/2024.
//
#pragma once

#include <Features/Modules/Module.hpp>
#include <Features/Modules/Setting.hpp>

class TestModule : public ModuleBase<TestModule> {
public:
    enum class Mode {
        DebugCameraTest,
        PathTest,
        Regen,
        None,
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode to run the module in", Mode::None, "Debug Camera Test", "Path Test", "Regen", "None");
    NumberSetting mProgress = NumberSetting("Progress", "The minimum progress to rotation", 1.f, 0.f, 1.f, 0.1f);
#ifdef __DEBUG__
    BoolSetting mShowDebugUi = BoolSetting("Show Debug UI", "Whether to show the debug UI", false);
#endif

    TestModule() : ModuleBase("TestModule", "A module for testin purposes", ModuleCategory::Misc, 0, false) {
        addSettings(
            &mMode,
            &mProgress
#ifdef __DEBUG__
            , &mShowDebugUi
#endif
        );

        mNames = {
            {Lowercase, "testmodule"},
            {LowercaseSpaced, "test module"},
            {Normal, "TestModule"},
            {NormalSpaced, "Test Module"}
        };
    }

    int64_t mLastLagback = 0;
    glm::ivec3 mRotatePos = { 0, 0, 0 };
    bool mShouldRotate = false;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onLookInputEvent(class LookInputEvent& event);
    void onRenderEvent(class RenderEvent& event);
};