#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"

class PacketFly : public IModule
{
private:
    std::string name = "PacketFly";
    std::vector<vec3_t> blockBelow;
    float speed = 0.325f;
    float Xspeed = 0.325f;
    float Zspeed = 0.325f;
    float up = 0.6f;
    float down = 0.6f;
    float glideMod = -0.00f;
    float glideModEffective = 0;
    int vanillaTPS = 20;
    vec3_t oldPos;
    bool NoPacket = false;
    bool antikick = false;
    bool trails = false;
    bool laggy = false;
    SettingEnum mode = this;
    SettingEnum phaseType = this;
    SettingEnum timer = this;

public:
    PacketFly();
    ~PacketFly();

    virtual const char *getRawModuleName();
    virtual const char *getModuleName() override;
    virtual void onTick(C_GameMode *gm);
    virtual void onMove(C_MoveInputHandler *input);
    virtual void onLevelRender();
    virtual void onEnable();
    virtual void onDisable();
};
