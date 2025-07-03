#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Burrow : public IModule
{
private:
    SettingEnum mode;
    SettingEnum rotation;
    SettingEnum blockPriority;
    SettingEnum blockType;
    SettingEnum swap;
    bool groundOnly = true;
    bool hidden = false;
    bool disableComplete = true;
    bool lerping = false;
public:
    void validBlock();
    void activate();
    virtual void onSendPacket(C_Packet *packet) override;
    virtual const char *getRawModuleName() override;
    virtual void onMove(C_MoveInputHandler* input);
    virtual void onTick(C_GameMode *gm) override;
    virtual void onDisable() override;
    Burrow();
};
