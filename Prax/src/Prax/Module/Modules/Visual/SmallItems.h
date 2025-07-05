//
// Created by vastrakai on 4/19/2024.
//


#pragma once

#include <Prax/Module/Module.h>

class SmallItems : public ModuleBase<SmallItems> {
public:
    SmallItems() : ModuleBase("SmallItems", "Makes items smaller", "Visual") {};

    void OnRenderMatrix(glm::mat4* matrix) override;

};