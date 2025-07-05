#pragma once

#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include "src/Prax/Module/Module.h"

#include <windows.h>
#include <wmp.h>
#include <comutil.h> // Add this for _bstr_t



class TestModule : public ModuleBase<TestModule> {
public:

    ModeSetting Mode = ModeSetting("Mode", "da mode", {"NiggaNuts"}, 0);
    NumberSetting Timer = NumberSetting("Timer", "Timer", 0, 40, 32, 1);
    NumberSetting Speed = NumberSetting("Speed", "Speed", 0, 1.f, 0.361065f, 1);
    BoolSetting DebugUi = BoolSetting("DebugUi", "Debug UI", false);
    BoolSetting NaClHud = BoolSetting("NaCl HUD", "HUD concept for NaCl, now on Prax", false);
    BoolSetting TestBool = BoolSetting("TestBool", "Test Bool", false);
    NumberSetting TestNumber = NumberSetting("TestNumber1", "Test Number", 0, 60, 1, 1, &TestBool);
    NumberSetting TestNumber2 = NumberSetting("TestNumber2", "Test Number 2", 0, 0, 100, 1, &TestBool);

    TestModule() : ModuleBase("TestModule", "For testing", "Misc", 0, false) {
        AddSetting(&Mode);
        Mode.Display = true;
        AddSetting(&DebugUi);
        AddSetting(&Timer);
        AddSetting(&Speed);
#ifndef __RELEASEFREE__
        AddSetting(&NaClHud);
        AddSetting(&TestBool);
        AddSetting(&TestNumber);
        AddSetting(&TestNumber2);
#endif
    }

    static int Tick;

    void OnEnable() override;
    void OnDisable() override;
    void OnRender() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
    void OnTick() override;


};