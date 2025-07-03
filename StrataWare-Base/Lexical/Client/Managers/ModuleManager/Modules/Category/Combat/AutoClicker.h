#pragma once
#include "../../ModuleBase/Module.h"
class AutoClicker : public Module {
private:
    int delay = 0;
    bool weapons = true;
    bool breakBlocks = true;
    bool rightclick = false;
    bool hold = false;
    int Odelay = 0;
    int consecutiveErrors = 0;
    std::chrono::steady_clock::time_point lastErrorTime;
    void handleLeftClick(LocalPlayer* gm, LocalPlayer* localPlayer);
    void handleRightClick(LocalPlayer* gm, LocalPlayer* localPlayer);
    bool safeAttack(LocalPlayer* gm, Actor* target);
    bool safeBlockBreak(LocalPlayer* gm, const BlockPos& pos, uint8_t face);
    bool safeBlockPlace(LocalPlayer* gm, const BlockPos& pos, uint8_t face);
    void resetState();
public:
    AutoClicker();
    void onNormalTick(LocalPlayer* gm) override;
    void onDisable() override;
};