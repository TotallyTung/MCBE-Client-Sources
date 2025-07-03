#pragma once
#include "../Module.h"
class BetterBow : public IModule
{
private:
    // bool silent = true;
    // bool predict = false;
    bool visualize = false;
    vec2_t angle;
    bool spamBow = true;
    int charge = 7;

    int i = 0;

public:
    bool shotOnce = false;
    SettingEnum aimbotType;
    BetterBow();
    ~BetterBow();

    virtual const char *getRawModuleName() override;
    virtual void onTick(C_GameMode *gm) override;
    virtual void onPostRender(C_MinecraftUIRenderContext *renderCtx) override;
    virtual void onSendPacket(C_Packet *packet) override;
};
