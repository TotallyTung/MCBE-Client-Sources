//
// Created by vastrakai on 11/12/2023.
//


#pragma once
#include <Prax/Module/Module.h>

class ReachDisplay : public ModuleBase<ReachDisplay> {
public:
    ReachDisplay() : ModuleBase("ReachDisplay", "Shows your reach", "Visual", 0, false) {}

    void OnEnable() override;
    void OnDisable() override;
    void OnAttack(Actor* actor);
    void OnTick() override;
};
