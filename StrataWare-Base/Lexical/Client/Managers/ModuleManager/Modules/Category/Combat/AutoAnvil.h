
#pragma once
#include "../../ModuleBase/Module.h"
class AutoAnvil : public Module {
private:
    bool mobs = false;
    int yoffsetJ = 3;
    int tickCounter = 0;
    int delay = 0;
public:
    AutoAnvil();
    void onDisable() override;
    void onEnable() override;
    void onNormalTick(LocalPlayer* localPlayer) override;
};