#pragma once
//
// Created by vastrakai on 8/26/2023.
//


#include <Prax/Module/Module.h>

class NoJumpDelay : public ModuleBase<NoJumpDelay> {
public:
    NoJumpDelay() : ModuleBase("NoJumpDelay", "Removes the delay between jumps", "Movement", 0, false) {}

    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;

};