#pragma once
//#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class Killaura : public IModule {
private:
    int slotKA = 0;
    int slotKASmart = 0;
    void findWeapon();
    bool boost = false;
    int APS = 20;
    int tps = 24;
    int RotSpeed = 20;
    int stoph = 0;
    int Timer = 20;
    int packetattack = 5;
    bool StopOnEat = false;
    bool swing = true;
    bool cps = false;
    bool hurttime = false;
    int slot;
    void findWeaponRain();
    void findWeaponWater();
    void findWeaponSmart();
    int Odelay = 0;
    int packet = 5;
public:
    bool hurttimeka = false;
    bool pkt = false;
    bool targetListEmpty = true;
    bool visualize = true;
    int rotationSpeed = 10.f;
    bool swingKA = true;
    bool Critical = true;
    bool hold = false;
    vec2_t localAngle = {};
    int n = 0;
    vec2_t angle;
    SettingEnum selectmode;
    int apsBoost = 1;
    float attackrange = 6.f;
    bool mobs = false;
    bool canswing = false;
    float range = 8.f;
    //Meow
    bool RotMeow = false;
    bool MeowRot = false;

    vec2_t tro;

    std::string name = "Killaura";
    bool rotation = this;
    SettingEnum mode = this;
    SettingEnum AutoWeapon = this;
    SettingEnum metarots = this;
    SettingEnum priority;
    SettingEnum rotations;
    SettingEnum mainW = this;
    SettingEnum timer = this;

    SettingEnum rotations1;
    Killaura();

    // Inherited via IModule

    virtual void onSendPacket(C_Packet* packet);
    void onGetPickRange(C_GameMode* gm);
    virtual void onPlayerTick(C_Player* plr);
    //virtual void onPlayerTick2(C_Player* plr);
    virtual const char* getRawModuleName();
    virtual const char* getModuleName();
    void onRotation(C_GameMode* gm);
    virtual void onTick(C_GameMode* gm);

    virtual void onDisable();
    virtual void onEnable();
    virtual void onKeyUpdate(int keyb, bool isDown) override;
    void doCritical();
};