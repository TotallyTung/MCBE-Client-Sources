//
// Created by Flash on 7/11/2023.
//

#pragma once


#include "Prax/Module/Module.h"

class FastStop : public ModuleBase<FastStop> {
public:
    FastStop() : ModuleBase("FastStop", "Stops you from moving instantly on key release", "Movement", 0, false) {}

    void OnTick() override;
};