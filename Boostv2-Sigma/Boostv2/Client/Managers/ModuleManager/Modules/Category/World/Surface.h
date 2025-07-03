#pragma once
#include "../../ModuleBase/Module.h"

class Surface : public Module {
public:
    Surface();

    void onEnable() override;
    void onNormalTick(LocalPlayer* localPlayer) override;
    void onDisable() override;

private:
    Vec3<float> targetPos;
    bool lerping = false;
};
