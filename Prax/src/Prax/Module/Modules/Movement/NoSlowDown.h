#pragma once
//
// Created by vastrakai on 10/13/2023.
//


#include <Prax/Module/Module.h>

class NoSlowDown : public ModuleBase<NoSlowDown> {
public:
    NoSlowDown() : ModuleBase("NoSlowDown", "Prevents items/blocks from slowing you down", "Movement", 0, false) {}

    void OnTick() override;
    void OnEnable() override;
    void OnDisable() override;
};