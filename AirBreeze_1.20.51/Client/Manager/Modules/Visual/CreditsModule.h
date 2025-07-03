#pragma once

#include "../../Module.h"

class CreditsModule : public Module {
public:
    CreditsModule(Category* c) : Module(c, "Credits", "Credits") {}
    ~CreditsModule() = default;

    void onImGuiRender() override;
};