#pragma once
#include "../../ModuleBase/Module.h"
#include "../Player/AutoMine.h"  // Include AutoMine for dependency resolution

class AfkMine : public Module {
private:
    int range = 6;  // Range for scanning ores
    int tickDelay = 2;  // Delay between ticks
    int delayCounter = 0;  // Counter for tick delay
    BlockPos currentTarget = BlockPos(0, 0, 0);  // Position of the current target block
    bool breaking = false;  // Indicates if we are currently breaking a block
    bool spoofSwitch = false;  // For handling certain switch states (can be further clarified)

    bool diamond = true;  // Should we break diamonds?
    bool iron = false;  // Should we break iron?
    bool gold = false;  // Should we break gold?
    bool coal = false;  // Should we break coal?
    bool redstone = false;  // Should we break redstone?
    bool emerald = false;  // Should we break emeralds?
    bool ancient = false;  // Should we break ancient debris?
    std::vector<Vec3<float>> itemTPList;
    int currentItem = 0;
    bool collectingItems = false;

public:
    AfkMine();  // Constructor
    void onNormalTick(LocalPlayer* localPlayer) override;  // Called every tick to perform actions
    void onDisable();
    virtual void onEnable() override;  // Called when the module is enabled
};