#pragma once

#include "../../Module.h"

class Scaffold : public Module {
public:
    Scaffold(Category* c) : Module(c, "Scaffold", "Basic scaffold that doesn't work on servers that use blockside") {
        this->key = 'R';
    }

public:
    Scaffold();

    void onBaseTick(Actor*, Level*, GameMode*) override;
};