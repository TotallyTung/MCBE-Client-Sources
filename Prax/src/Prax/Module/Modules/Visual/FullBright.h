//
// Created by vastrakai on 12/27/2023.
//


#pragma once
#include <Prax/Module/Module.h>

class FullBright : public ModuleBase<FullBright> {
public:
    FullBright() : ModuleBase("FullBright", "Makes the game brighter", "Visual", 0, false){}

    static float OldGamma;

    void OnEnable() override;
    void OnDisable() override;

};
