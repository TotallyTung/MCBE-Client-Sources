#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class Anchor : public IModule
{
private:
public:
    bool obsidian = true;
    bool bedrock = true;
    int range = 5;
    int rangeY = 5;

    Anchor();
    ~Anchor();
    // Inherited via IModule
    virtual const char *getRawModuleName() override;
    virtual void onTick(C_GameMode *gm) override;
    virtual void onMove(C_MoveInputHandler *input) override;
};