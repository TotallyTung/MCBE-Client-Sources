#pragma once

#include "../../Module.h"

class ClickGui : public Module {
public:
    ClickGui(Category* c) : Module(c, "ClickGui", "Clickgui, so you can click stuff!") {
        this->key = VK_INSERT;
    };

    auto onImGuiRender() -> void override;
    auto onEnable() -> void override;
    auto onDisable() -> void override;
    void onKey(uint64_t key, bool isDown, bool* cancelSend) override;
};