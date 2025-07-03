#pragma once
#include "../Module.h"

class TpAura : public Module {
private:
    int range = 50;          // Teleport range
    int yOffset = 0;         // Adjustable Y offset
    Actor* currentTarget = nullptr;
    Vec3<float> targetPos;   // Updated enemy position

    // We'll use targetListJ for finding enemies
    // (It is declared as static in the .cpp file)

    // Helper: Finds valid enemy targets and stores them in targetListJ.
    void findEntityhhD();

public:
    TpAura();
    virtual void onEnable() override;
    virtual void onNormalTick(Actor* actor) override;
    virtual void onDisable() override;
};