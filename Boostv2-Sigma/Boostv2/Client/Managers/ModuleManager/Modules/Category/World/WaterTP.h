#pragma once
#include "../../ModuleBase/Module.h"
#include <unordered_set>

class WaterTP : public Module {
public:
    WaterTP();

    void onEnable() override;
    void onNormalTick(LocalPlayer* localPlayer) override;
    void onDisable() override;

private:
    Vec3<float> goal;                // Target position (1 block above water)
    float closestDistance;           // Distance to the closest water block
    bool tpdone = false;             // If teleportation is done
    bool works = false;              // If the module works 
    int range;                       // Search range for water blocks
    std::unordered_set<BlockPos> visitedWater;  // To store already visited water blocks (optional)
};
