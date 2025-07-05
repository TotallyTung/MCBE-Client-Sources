#pragma once
//
// Created by vastrakai on 10/13/2023.
//


#include <Prax/Hook/Hook.h>

class ActorSwing : public Hook {
public:
    ActorSwing() : Hook("ActorSwing") {
        InitOnConstruct = false;
    }


    void Init() override;
    void UnHook() override;
};