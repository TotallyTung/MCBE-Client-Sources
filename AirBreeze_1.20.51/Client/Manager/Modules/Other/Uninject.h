#pragma once

#include "../../Module.h"

class UninjectModule : public Module {
public:
    UninjectModule(Category* c) : Module(c, "Uninject", "Uninject") {}
    ~UninjectModule() = default;

    void onEnable() override;
};