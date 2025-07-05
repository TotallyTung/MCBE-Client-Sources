//
// Created by Flash on 9/6/2023.
//

#pragma once


#include "src/Prax/SDK/Game/Minecraft.h"
#include "Prax/Module/ModuleManager.h"

class DimensionH : public Hook {
public:
    DimensionH() : Hook("Dimension") {
        InitOnConstruct = false;
    }

    void Init() override;
    void UnHook() override;
};


