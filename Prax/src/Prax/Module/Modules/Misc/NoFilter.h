//
// Created by vastrakai on 12/23/2023.
//


#pragma once
#include <Prax/Module/Module.h>

class NoFilter : public ModuleBase<NoFilter>
{
    bool wasEnabled = false;

public:
    NoFilter() : ModuleBase("NoFilter", "Disables the Minecraft chat filter.", "Misc", 0, true) {};

    void OnEnable() override;
    void OnTick() override;
    void OnDisable() override;
};
