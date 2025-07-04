#pragma once
#include "../../ModuleBase/Module.h"

class Killaura : public Module {
public:
    bool mobs = false;
    bool rotate = false;
    float range = 3.7f;
    int hitattemptsFE = 3;
    int delayOFE = 0;
    int delayFE = 0;
    bool useHookspeed = false;
    bool arewehooking = false;
    float hookspeedH = 1.f;
    Killaura();
    void onDisable() override;
    void onEnable() override;
    void onNormalTick(LocalPlayer* localPlayer) override;
    void onUpdateRotation(LocalPlayer* localPlayer) override;
    void onSendPacket(Packet* packet) override;
};
