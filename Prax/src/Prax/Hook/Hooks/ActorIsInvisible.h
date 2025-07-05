#pragma once
//
// Created by vastrakai on 9/25/2023.
//


#include <Prax/Hook/Hook.h>

class ActorIsInvisible : public Hook {
public:
    ActorIsInvisible() : Hook("ActorIsInvisible") {
        InitOnConstruct = false;
    }

    void Init() override;
    void UnHook() override;

};