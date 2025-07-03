#pragma once

#include "../../Module.h"

class Killaura : public Module {
public:
    Killaura(Category* c) : Module(c, "Killaura", "Simple killaura that attack everything") {
        this->key = 'P';
        Float("Reach", 1.1f, 4.5f, &maxTargetDistance);
        Int("CPS", 50.f, 200.0f, &cpsDelay);
    }

public:
    Killaura();

    void onBaseTick(Actor*, Level*, GameMode*) override;

    void onImGuiRender();

    float maxTargetDistance = 1.1f; // Maximum target distance in blocks
    int cpsDelay = 100; // Delay between clicks for 10 CPS (in milliseconds)


private:
    char searchQuery[256];
    char h[2];


private:
    float range = 7.f;
};