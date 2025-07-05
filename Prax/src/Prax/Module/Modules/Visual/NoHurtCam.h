//
// Created by vastrakai on 12/28/2023.
//


#pragma once

#include <Prax/Module/Module.h>

class NoHurtCam : public ModuleBase<NoHurtCam> {
public:
    NoHurtCam() : ModuleBase("NoHurtCam", "Disables the hurt camera effect", "Visual", 0, false) {}

    void OnTick() override;
    void OnRender() override;
};