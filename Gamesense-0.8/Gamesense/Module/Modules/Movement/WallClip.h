#pragma once
#include "../Module.h"

class WallClip : public IModule
{
private:
    float offset = 0.1f;
    float range = 0.5f;

public:
    SettingEnum mode = this;
    WallClip();
    ~WallClip();

    virtual const char *getRawModuleName() override;
    virtual void onTick(C_GameMode *gm) override;
    virtual void onSendPacket(C_Packet *packet) override;
};
