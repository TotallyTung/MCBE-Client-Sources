#pragma once

#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include "../../Module.h"

class BPSDisplay : public ModuleBase<BPSDisplay> {
public:

    BoolSetting boolSetting = BoolSetting("Average", "Averages your BPS over the last second", false);
    /*
    BoolSetting graphSetting = BoolSetting("Graph", "Whether or not to show your bps over time", false);
    */
    ModeSetting displayMode = ModeSetting("Display Mode", "How to display your BPS", {"Text", "Graph", "Both"}, 0);
    ModeSetting positionSetting = ModeSetting("Position", "Where to display your BPS", {"Top Middle", "Middle Hotbar"}, 0);
    BoolSetting rgb = BoolSetting("RGB", "Whether or not to use RGB", false);

    BPSDisplay() : ModuleBase("BPSDisplay", "Displays your BPS in the left-hand corner of the screen", "Visual", 66, false)  {
        AddSetting(&boolSetting);
        AddSetting(&displayMode);
        AddSetting(&positionSetting);
        AddSetting(&rgb);
    }


    static float bps;
    static float lastBps;
    static float avgBps;

    void OnEnable() override;
    void OnDisable() override;
    void DrawSpeedometer(ImVec2 pos, float radius, float speed, float maxSpeed);
    void OnRender() override;
    void OnTick() override;

};