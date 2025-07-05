#pragma once
//
// Created by vastrakai on 9/24/2023.
//


#include <Prax/Hook/Hook.h>

class ActorSetNametag : public Hook {
public:
    ActorSetNametag() : Hook("ActorSetNametag") {
        InitOnConstruct = false;
    }

    void Init() override;
    void UnHook() override;

};