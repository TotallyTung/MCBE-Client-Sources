#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../Utils/TargetUtil.h"

class KillauraRF : public IModule {
private:
    bool isMulti = true;
    bool isMulti2 = true;
    int delay = 0;
    int packet = 0;
    int slotKA = 0;
    int timer = 20;
    int packet2 = 0;
    int Odelay = 0;
    bool autoweapon = false;
    void findWeapon();
    bool silent = false;
    bool noSilent = false;
    bool longstrafe = false;
    bool VisTarget = false;

    SettingEnum RotMode;

public:
    bool isMobAura = false;
    bool hurttime = true;
    float range = 6;
    int timer5 = 0;


    KillauraRF();
    ~KillauraRF();

    // Inherited via IModule
    virtual const char* getModuleName() override;
    virtual void onTick(C_GameMode* gm) override;
    virtual void onEnable() override;
    virtual void onDisable() override;
    virtual void onSendPacket(C_Packet* packet) override;
    virtual void onLevelRender() override;
};