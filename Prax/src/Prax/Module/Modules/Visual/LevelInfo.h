//
// Created by Flash on 1/11/2024.
//


#pragma once

#include <Prax/Module/Settings/NumberSetting.h>

#include "../../Module.h"

class LevelInfo : public ModuleBase<LevelInfo> {
public:
    BoolSetting ShowBps = BoolSetting("Show BPS", "Shows the BPS of the client", true);
    BoolSetting AverageBps = BoolSetting("Average BPS", "Shows the average BPS instead of the current BPS", true);
    BoolSetting ShowFps = BoolSetting("Show FPS", "Shows the FPS of the client", true);
    BoolSetting ShowPing = BoolSetting("Show Ping", "Shows the ping of the client", true);
    BoolSetting ShowPosition = BoolSetting("Show Position", "Shows the position of the player", true);
    BoolSetting ShowPearlCount = BoolSetting("Show Pearl Count", "Shows the amount of pearls thrown", true);
    NumberSetting SizeSetting = NumberSetting("Size", "Size of the text shown", 1, 50, 20, 1);

    LevelInfo() : ModuleBase("LevelInfo", "Shows information relating to the client and player", "Visual", 0, true) {
        AddSetting(&ShowBps);
        AddSetting(&AverageBps);
        AddSetting(&ShowFps);
        AddSetting(&ShowPing);
        AddSetting(&ShowPosition);
        AddSetting(&ShowPearlCount);
        AddSetting(&SizeSetting);
    }

    /* Static variables */
    static float Bps;
    static float LastBps;
    static float AvgBps;

    static std::map<uint64_t, float> BpsHistory;
    /* Module overrides */
    void OnTick() override;
    void OnRender() override;
};