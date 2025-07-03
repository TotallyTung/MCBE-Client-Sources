#pragma once
#include "../../ModuleBase/Module.h"

class AutoMine : public Module {
private:
    std::vector<BlockPos> targetListTP;
    bool works = false;

    // Ore targeting flags
    bool coal = false;
    bool dim = false;
    bool iron = false;
    bool gold = false;
    bool redstone = false;
    bool emerald = false;
    bool ancient = false;
    bool spawners = false;
    bool quartz = false;
    bool cuOre = false;
    int cuID = 1;

    // Misc options
    bool antiLava = false;
    bool antiBorder = false;
    bool antiGravel = false;
    int clusterSize = 5;

    // Teleport settings
    bool TP = false;
    bool tpdone = false;
    Vec3<float> goal;
    float closestDistance = 0;

    // Advanced/legacy options
    bool entity = false;
    bool border = true;
    bool al = true;
    bool ab = true;
    bool as = true;
    int range = 50;
    int range2 = 4;
    int speed = 1;
    int timerV = 480;
    int why = 2;
    int breakKeybind = 0x0;
    bool midClickTP = false;


public:
    AutoMine();
    virtual void onEnable() override;
    virtual void onNormalTick(LocalPlayer* actor) override;
    void onDisable();
};