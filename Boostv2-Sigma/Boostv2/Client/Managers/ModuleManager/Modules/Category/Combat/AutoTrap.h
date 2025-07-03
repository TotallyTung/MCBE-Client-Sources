#pragma once
#include "../../ModuleBase/Module.h"
#include <unordered_set>

class AutoTrap : public Module {
private:
    int tickCounter = 0;
    int placeDelay = 0;
    bool targetMobs = false;
    bool swap = true;
    bool silentSwap = false;
    bool rotate = true;
    bool airCheck = true;
    int trapMode = 0;
    void placeBlock(LocalPlayer* localPlayer, BlockPos pos, int face);
    std::unordered_set<BlockPos> placedBlocks;
    int findBestPlacementFace(BlockSource* region, const BlockPos& pos);
    Vec2<float> computePlaceAngle(const Vec3<float>& from, const Vec3<float>& to);
public:
    AutoTrap();
    void onDisable() override;
    void onEnable() override;
    void onNormalTick(LocalPlayer*) override;
};