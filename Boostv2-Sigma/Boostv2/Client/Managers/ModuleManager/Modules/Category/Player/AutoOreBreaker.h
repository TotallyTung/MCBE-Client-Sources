#pragma once
#include "../../ModuleBase/Module.h"

class AutoOreBreaker : public Module {
public:
    int range = 6;
    int tickDelay = 2;
    int delayCounter = 0;
    BlockPos currentTarget = BlockPos(0, 0, 0);
    bool breaking = false;
    bool spoofSwitch = false;

    bool diamond = true;
    bool iron = false;
    bool gold = false;
    bool coal = false;
    bool redstone = false;
    bool emerald = false;
    bool ancient = false;

public:
    AutoOreBreaker();
    void onEnable();
    bool breakListEmpty = true;
    bool isEmpty() const;
    void onNormalTick(LocalPlayer* localPlayer) override;
};
