#pragma once
#include "../../ModuleBase/Module.h"
#include <set>

class ChestTP : public Module {
private:
    std::set<BlockPos> visitedChests;
    bool works = false;
    bool tpdone = false;
    Vec3<float> goal;
    float closestDistance = 0;
    int range = 50;

public:
    ChestTP();
    virtual void onEnable() override;
    virtual void onNormalTick(LocalPlayer* actor) override;
    virtual void onDisable() override;
};
