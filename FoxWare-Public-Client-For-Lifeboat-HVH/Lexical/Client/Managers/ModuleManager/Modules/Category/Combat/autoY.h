#pragma once
#include "../../ModuleBase/Module.h"

class AutoY : public Module {
public:
    bool mobs = false;
    float range = 3.7f;
    AutoY();
    void onDisable() override;
    void onEnable() override;
    void onNormalTick(LocalPlayer* localPlayer) override;
    void onUpdateRotation(LocalPlayer* localPlayer) override;
    void onSendPacket(Packet* packet) override;
};
