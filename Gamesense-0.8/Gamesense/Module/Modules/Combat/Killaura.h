#pragma once
// #include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class Killaura : public IModule
{
private:
    int slotKA = 0;
    int slotKASmart = 0;
    void findWeapon();
    void findWeaponSmart();
    int delayka = 0;
    int Odelay = 0;

public:
    bool hurttimeka = false;
    bool targetListEmpty = true;
    bool visualize = true;
    bool showArray = true;
    bool ReturnOnEat = true;
    bool swingKA = true;
    bool hold = false;
    bool mobs = false;
    bool autoDisable = false;
    float tps = 20.f;

    float range = 8.f;

    vec2_t tro;

    const char* name = "Aura";
    SettingEnum rotations = this;
    SettingEnum mode = this;
    SettingEnum AutoWeapon = this;
    SettingEnum renderType;
    SettingEnum mainW = this;
    SettingEnum timer = this;

    Killaura();

    // Inherited via IModule
    virtual void onSendPacket(C_Packet *packet);
    virtual void onPlayerTick(C_Player *plr);
    virtual const char *getRawModuleName();
    virtual const char *getModuleName();
    virtual const char* getModName();
    virtual void onTick(C_GameMode *gm);
    virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
    virtual void onLevelRender();
    virtual void onDisable();
    virtual void onEnable();
};