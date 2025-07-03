#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class ClientSetting : public IModule
{
private:
public:
    bool creds = true;
    bool mm = true;
    bool changelog = true;
    bool injectMsg = true;
    bool playSound = true;
    bool nameSpoof = false;
    bool logs = false;
    bool welcome = true;
    SettingEnum ejectMsg;
    SettingEnum autoSave;
    virtual const char *getRawModuleName() override;
    virtual void onPostRender(C_MinecraftUIRenderContext *renderCtx) override;
    virtual void onTick(C_GameMode *gm);
    virtual void onDisable();
    ClientSetting();
};
