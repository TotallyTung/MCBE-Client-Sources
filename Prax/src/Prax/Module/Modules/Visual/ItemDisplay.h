//
// Created by Flash on 12/27/2023.
//


#pragma once

#include "../../Module.h"

class ItemDisplay : public ModuleBase<ItemDisplay> {
    public:

    ItemDisplay() : ModuleBase("ItemDisplay", "Shows the item you are holding", "Visual", 0, true) {};

    void OnEngineRender(MinecraftUIRenderContext* ctx, Font* font) override;
};