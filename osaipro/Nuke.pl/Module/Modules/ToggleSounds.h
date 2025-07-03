#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class ToggleSounds : public IModule {
public:
    float volume = 1.f;
    SettingEnum sound = this;
    virtual const char* getModuleName() override;
    ToggleSounds();
};