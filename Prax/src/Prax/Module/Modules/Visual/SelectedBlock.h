//
// Created by Flash on 7/11/2023.
//

#pragma once


#include "Prax/Module/Module.h"

class SelectedBlock : public ModuleBase<SelectedBlock> {
public:
    SelectedBlock() : ModuleBase("SelectedBlock", "View the selected block", "Visual", 0, false) {}

    void OnRender() override;
};