//
// Created by vastrakai on 2/9/2024.
//


#pragma once
#include <Prax/Module/Module.h>

class InvMove : public ModuleBase<InvMove> {
public:
    InvMove() : ModuleBase("InvMove", "Lets you move while in GUIs", "Movement") {
    }

    static inline bool JumpOverride = false;

    void OnEnable();
    void OnDisable();
    void OnTick() override;


};
