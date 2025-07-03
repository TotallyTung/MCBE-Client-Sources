#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"

class Jetpack : public IModule
{
private:
    int delay = 0;
    bool isBypass = false;
    float speedMod = 1;
    bool hold = false;
    bool onW = false;
    bool fsIsOn;

public:
    C_MovePlayerPacket *jetpack = nullptr;
    Jetpack();
    ~Jetpack();

    virtual void onEnable();
    virtual void onDisable();
    virtual void onMove(C_MoveInputHandler* input);
    virtual void onTick(C_GameMode *gm);
    virtual bool isHoldMode();

    // Inherited via IModule
    virtual const char *getRawModuleName();
};
